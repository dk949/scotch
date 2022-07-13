#include "wasm_compiler.hpp"

#include "ast.hpp"
#include "macros.hpp"
#include "type.hpp"

#include <fmt/format.h>
#include <ranges>


ErrorOr<Type> WasmCompiler::getType(Expr *expr) {
    auto &t = expr->overallType();
    if (!t)
        TRY_VOID(typeCheckExpr(expr));
    return *t;
}

ErrorOr<void> WasmCompiler::typeCheckExpr(Expr *expr) {
    if (const auto *assign = dynamic_cast<const Assign *>(expr)) {
        if (!m_symbolTable.contains(assign->target())) {
            return tl::unexpected(Error {fmt::format("{} does not name a variable", assign->target())});
        }
        auto targetType = m_symbolTable.at(assign->target());
        auto targetMod = m_modTable.at(assign->target());
        if (targetMod == Mod::CONST) {
            return tl::unexpected(Error {fmt::format("cannot assign to const variable {}", assign->target())});
        }
        auto sourceType = TRY(getType(assign->source().get()));
        expr->overallType() = sourceType;
        if (targetType.type != sourceType) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expressino of type {} to variable {} of type {}",
                    m_ts.string(sourceType),
                    assign->target(),
                    m_ts.string(targetType.type))});
        }
    } else if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        if (m_symbolTable.contains(declare->name())) {
            return tl::unexpected(Error {fmt::format("Redeclaration of variable {}", declare->name())});
        }
        auto sourceType = TRY(getType(declare->source().get()));
        expr->overallType() = sourceType;
        if (declare->declaredType() && (declare->declaredType() != sourceType)) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expression of type {} to variable {} of type {}",
                    m_ts.string(sourceType),
                    declare->name(),
                    m_ts.string(*declare->overallType()))});
        }
        m_symbolTable[declare->name()] = Symbol {.kind = SymbolKind::Variable, .type = sourceType};
        m_modTable[declare->name()] = declare->mod();
    } else if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        auto valueType = TRY(getType(ret->value().get()));
        expr->overallType() = valueType;
        auto funcRet = m_symbolTable.at(m_currentFunc).type;
        if (funcRet != valueType) {
            return tl::unexpected(
                Error {fmt::format("cannot return expression of type {} from function {} returning {}",
                    m_ts.string(valueType),
                    m_currentFunc,
                    m_ts.string(funcRet))});
        }
    } else if (auto *cond = dynamic_cast<Condition *>(expr)) {
        const auto *conditionType = TRY(getType(cond->conditon()));
    } else if (const auto *add = dynamic_cast<const Add *>(expr)) {
        auto lhsType = TRY(getType(add->lhs().get()));
        auto rhsType = TRY(getType(add->rhs().get()));
        expr->overallType() = lhsType;
        if (lhsType != rhsType) {
            return tl::unexpected(Error {
                fmt::format("cannot add expression of type {} to expression of type {}", m_ts.string(lhsType), m_ts.string(rhsType))});
        }
    } else if (const auto *varexpr = dynamic_cast<const VarExpr *>(expr)) {
        if (!m_symbolTable.contains(varexpr->name())) {
            return tl::unexpected(Error {fmt::format("{} does not name a variable", varexpr->name())});
        }
        expr->overallType() = m_symbolTable.at(varexpr->name()).type;
    } else if (const auto *lit = dynamic_cast<const BuiltInLiteral *>(expr)) {
        auto type = lit->toType();
        expr->overallType() = type;
    } else {
        return tl::unexpected(Error {fmt::format("{}: Unexpected expression type {}", compilerType(), expr->exprType())});
    }
    return {};
}


ErrorOr<void> WasmCompiler::checkFunctionInTable(const FunctionDef &func) {
    if (std::find_if(std::begin(m_symbolTable), std::end(m_symbolTable), [&func](auto &&a) {
            return a.first == func.name() && a.second.kind == SymbolKind::Function;
        }) != std::end(m_symbolTable)) {
        return tl::unexpected {Error {fmt::format("redeclaration of function {}", func.name())}};
    }

    return {};
}

ErrorOr<void> WasmCompiler::typeCheck() {
    m_symbolTable[m_program.mod().name()] = Symbol {SymbolKind::Module, emptyType};
    for (const auto &func : m_program.mod().funcs()) {
        TRY_VOID(checkFunctionInTable(func));
        m_symbolTable[func.name()] = Symbol {SymbolKind::Function, func.ret()};
        m_currentFunc = func.name();

        for (const auto &arg : func.args()) {
            m_symbolTable[arg.name()] = Symbol {SymbolKind::Argument, arg.type()};
            m_modTable[arg.name()] = Mod::CONST;
        }

        for (const auto &expr : func.body()) {
            TRY_VOID(typeCheckExpr(expr.get()));
        }
    }

    return {};
}



[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArg(const Arg &arg) const {
    return fmt::format("(param ${} {})", arg.name(), TRY(m_ts[arg.type()].asBuiltin()));
}


[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArgs(const std::vector<Arg> &args) const {
    std::string out;
    for (const auto &arg : args) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileArg(arg)));
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileEpxr(const Expr *expr) const {
    if (const auto *assign = dynamic_cast<const Assign *>(expr)) {
        return fmt::format("{}\n(local.set ${})\n", TRY(compileEpxr(assign->source().get())), assign->target());
    }
    if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        return fmt::format("{}\n(local.set ${})\n", TRY(compileEpxr(declare->source().get())), declare->name());
    }
    if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        return fmt::format("{}\nreturn\n", TRY(compileEpxr(ret->value().get())));
    }
    if (const auto *add = dynamic_cast<const Add *>(expr)) {
        return fmt::format("{}\n{}\n{}.add",
            TRY(compileEpxr(add->lhs().get())),
            TRY(compileEpxr(add->rhs().get())),
            std::get<BuiltInType>(expr->overallType().value().type()));
    }
    if (const auto *varexpr = dynamic_cast<const VarExpr *>(expr)) {
        return fmt::format("(local.get ${})", varexpr->name());
    }
    // FIXME: use repr
    if (const auto *boolean = dynamic_cast<const Bool *>(expr)) {
        return fmt::format("(i32.const {})", +boolean->value());
    }
    if (const auto *int32 = dynamic_cast<const Int32 *>(expr)) {
        return fmt::format("(i32.const {})", int32->value());
    }
    if (const auto *int64 = dynamic_cast<const Int64 *>(expr)) {
        return fmt::format("(i64.const {})", int64->value());
    }
    if (const auto *float32 = dynamic_cast<const Float32 *>(expr)) {
        return fmt::format("(f32.const {})", float32->value());
    }
    if (const auto *float64 = dynamic_cast<const Float64 *>(expr)) {
        return fmt::format("(f64.const {})", float64->value());
    }

    return tl::unexpected(Error {fmt::format("{}: Unexpected expression type {}", compilerType(), expr->exprType())});
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileVars() const {
    std::string out;
    for (const auto &[name, symbol] : m_symbolTable) {
        if (symbol.kind == SymbolKind::Variable) {
            fmt::format_to(std::back_inserter(out), "(local ${} {})\n", name, TRY(m_ts[symbol.type].asBuiltin()));
        }
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileBlock(const std::vector<std::shared_ptr<Expr>> &block) const {
    std::string out;
    for (const auto &stmt : block) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileEpxr(stmt.get())));
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileFunc(const FunctionDef &func) const {
    auto a = func.body();

    std::string out = fmt::format(  //
        "(func\n${0}\n"
        "(export \"{0}\")\n"
        "{2}\n"
        "(result {1})\n"
        "{3}"
        "{4}"
        ")\n",
        func.name(),
        TRY(m_ts[func.ret()].asBuiltin()),
        TRY(compileArgs(func.args())),
        TRY(compileVars()),
        TRY(compileBlock(func.body())));

    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileFuncs(const std::vector<FunctionDef> &funcs) const {
    std::string out;
    for (const auto &func : funcs) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileFunc(func)));
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileModule(const Module &mod) const {
    return fmt::format("(module\n${}\n{})", mod.name(), TRY(compileFuncs(mod.funcs())));
}

ErrorOr<std::string> WasmCompiler::compile() {
    return compileModule(m_program.mod());
}
