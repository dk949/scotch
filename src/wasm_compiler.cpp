#include "wasm_compiler.hpp"

#include "ast.hpp"
#include "macros.hpp"
#include "type.hpp"

#include <fmt/format.h>
#include <ranges>


ErrorOr<Type> WasmCompiler::getType(std::string_view funcname, Expr *expr) {
    auto &t = expr->overallType();
    if (!t)
        TRY_VOID(typeCheckExpr(funcname, expr));
    return *t;
}

ErrorOr<void> WasmCompiler::typeCheckExpr(std::string_view funcName, Expr *expr) {
    if (const auto *assign = dynamic_cast<const Assign *>(expr)) {
        if (!m_symbolTable.contains(assign->target())) {
            return tl::unexpected(Error {fmt::format("{} does not name a variable", assign->target())});
        }
        auto targetType = m_symbolTable.at(assign->target());
        auto targetMod = m_modTable.at(assign->target());
        if (targetMod == Mod::CONST) {
            return tl::unexpected(Error {fmt::format("cannot assign to const variable {}", assign->target())});
        }
        auto sourceType = TRY(getType(funcName, assign->source().get()));
        expr->overallType() = sourceType;
        if (targetType.type != sourceType) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expressino of type {} to variable {} of type {}",
                    m_ts[sourceType],
                    assign->target(),
                    m_ts[targetType.type])});
        }
    } else if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        if (m_symbolTable.contains(declare->name())) {
            return tl::unexpected(Error {fmt::format("Redeclaration of variable {}", declare->name())});
        }
        auto sourceType = TRY(getType(funcName, declare->source().get()));
        expr->overallType() = sourceType;
        if (declare->declaredType() && (declare->declaredType() != sourceType)) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expression of type {} to variable {} of type {}",
                    m_ts[sourceType],
                    declare->name(),
                    m_ts[*declare->overallType()])});
        }
        m_symbolTable[declare->name()] = Symbol {.kind = SymbolKind::Variable, .type = sourceType};
        m_modTable[declare->name()] = declare->mod();
    } else if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        auto valueType = TRY(getType(funcName, ret->value().get()));
        expr->overallType() = valueType;
        // TODO: std::string_view m_currentFunc; // holds name of the currently processed function
        auto funcRet = m_symbolTable.at(Ident(funcName)).type;
        if (funcRet != valueType) {
            return tl::unexpected(Error {
                fmt::format("cannot return expression of type {} from function {} returning {}", m_ts[valueType], funcName, m_ts[funcRet])});
        }
    } else if (const auto *add = dynamic_cast<const Add *>(expr)) {
        auto lhsType = TRY(getType(funcName, add->lhs().get()));
        auto rhsType = TRY(getType(funcName, add->rhs().get()));
        expr->overallType() = lhsType;
        if (lhsType != rhsType) {
            return tl::unexpected(Error {
                fmt::format("cannot add expression of type {} to expression of type {}", m_ts[lhsType], m_ts[rhsType])});
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

ErrorOr<void> WasmCompiler::typeCheck() {
    m_symbolTable[m_program.mod().name()] = Symbol {SymbolKind::Module, emptyType};
    for (const auto &func : m_program.mod().funcs()) {
        m_symbolTable[func.name()] = Symbol {SymbolKind::Function, func.ret()};

        for (const auto &arg : func.args()) {
            m_symbolTable[arg.name()] = Symbol {SymbolKind::Argument, arg.type()};
            m_modTable[arg.name()] = Mod::CONST;
        }

        for (const auto &expr : func.body()) {
            // TRY_VOID(getSymbolsFromExpression(expr.get()));
            TRY_VOID(typeCheckExpr(func.name(), expr.get()));
        }
    }

    return {};
}



[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArg(const Arg &arg) const {
    return fmt::format("(param ${} {})", arg.name(), m_ts[arg.type()]);
}


[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArgs(const std::vector<Arg> &args) const {
    std::string out;
    for (const auto &arg : args) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileArg(arg)));
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileEpxr(std::string_view funcName, const Expr *expr) const {
    if (const auto *assign = dynamic_cast<const Assign *>(expr)) {
        return fmt::format("{}\n(local.set ${})\n", TRY(compileEpxr(funcName, assign->source().get())), assign->target());
    }
    if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        return fmt::format("{}\n(local.set ${})\n", TRY(compileEpxr(funcName, declare->source().get())), declare->name());
    }
    if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        return fmt::format("{}\nreturn\n", TRY(compileEpxr(funcName, ret->value().get())));
    }
    if (const auto *add = dynamic_cast<const Add *>(expr)) {
        return fmt::format("{}\n{}\n{}.add",
            TRY(compileEpxr(funcName, add->lhs().get())),
            TRY(compileEpxr(funcName, add->rhs().get())),
            expr->overallType().value());
    }
    if (const auto *varexpr = dynamic_cast<const VarExpr *>(expr)) {
        return fmt::format("(local.get ${})", varexpr->name());
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
            fmt::format_to(std::back_inserter(out), "(local ${} {})\n", name, m_ts[symbol.type]);
        }
    }
    return out;
}

[[nodiscard]] ErrorOr<std::string>
    WasmCompiler::compileBlock(std::string_view funcName, const std::vector<std::shared_ptr<Expr>> &block) const {
    std::string out;
    for (const auto &stmt : block) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileEpxr(funcName, stmt.get())));
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
        m_ts[func.ret()],
        TRY(compileArgs(func.args())),
        TRY(compileVars()),
        TRY(compileBlock(func.name(), func.body())));

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
