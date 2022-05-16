#include "wasm_compiler.hpp"

#include "ast.hpp"
#include "macros.hpp"

#include <fmt/format.h>
#include <ranges>

ErrorOr<void> WasmCompiler::getSymbolsFromExpression(const Expr *expr) {
    if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        m_symbolTable[declare->target().name()] = Symbol {SymbolKind::Variable, declare->target().type()};
    }
    return {};
}


ErrorOr<Ident> WasmCompiler::typeCheckExpr(std::string_view funcName, const Expr *expr) {
    if (const auto *assign = dynamic_cast<const Assign *>(expr)) {
        if (!m_symbolTable.contains(assign->target())) {
            return tl::unexpected(Error {fmt::format("{} does not name a variable", assign->target())});
        }
        auto targetType = m_symbolTable.at(assign->target());
        if (targetType.type.mod() == Mod::CONST) {
            return tl::unexpected(Error {fmt::format("cannot assign to const variable {}", assign->target())});
        }
        auto sourceType = TRY(typeCheckExpr(funcName, assign->source().get()));
        if (targetType.type.name() != sourceType) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expressino of type {} to variable {} of type {}",
                    sourceType,
                    assign->target(),
                    targetType.type.name())});
        }
        return targetType.type.name();
    }
    if (const auto *declare = dynamic_cast<const Declare *>(expr)) {
        auto sourceType = TRY(typeCheckExpr(funcName, declare->source().get()));
        if (declare->target().type().name() != sourceType) {
            return tl::unexpected(
                Error {fmt::format("cannot assign the expressino of type {} to variable {} of type {}",
                    sourceType,
                    declare->target().name(),
                    declare->target().type().name())});
        }
        return declare->target().type().name();
    }
    if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        auto valueType = TRY(typeCheckExpr(funcName, ret->value().get()));
        auto funcRet = m_symbolTable.at(Ident(funcName)).type.name();
        if (funcRet != valueType) {
            return tl::unexpected(Error {
                fmt::format("cannot return expression of type {} from function {} returning {}", valueType, funcName, funcRet)});
        }
        return valueType;
    }
    if (const auto *add = dynamic_cast<const Add *>(expr)) {
        auto lhsType = TRY(typeCheckExpr(funcName, add->lhs().get()));
        auto rhsType = TRY(typeCheckExpr(funcName, add->rhs().get()));
        if (lhsType != rhsType) {
            return tl::unexpected(
                Error {fmt::format("cannot add expression of type {} to expression of type {}", lhsType, rhsType)});
        }
        return lhsType;
    }
    if (const auto *varexpr = dynamic_cast<const VarExpr *>(expr)) {
        if (!m_symbolTable.contains(varexpr->name())) {
            return tl::unexpected(Error {fmt::format("{} does not name a variable", varexpr->name())});
        }
        return m_symbolTable.at(varexpr->name()).type.name();
    }
    if (dynamic_cast<const Int32 *>(expr)) {
        return "i32";
    }
    if (dynamic_cast<const Int64 *>(expr)) {
        return "i64";
    }
    if (dynamic_cast<const Float32 *>(expr)) {
        return "f32";
    }
    if (dynamic_cast<const Float64 *>(expr)) {
        return "f64";
    }

    return tl::unexpected(Error {fmt::format("{}: Unexpected expression type {}", compilerType(), expr->exprType())});
}

ErrorOr<void> WasmCompiler::typeCheck(const Program &prog) {
    m_symbolTable[prog.mod().name()] = Symbol {SymbolKind::Module, emptyType};
    for (const auto &func : prog.mod().funcs()) {
        m_symbolTable[func.name()] = Symbol {SymbolKind::Function, func.ret()};

        for (const auto &arg : func.args()) {
            m_symbolTable[arg.name()] = Symbol {SymbolKind::Argument, arg.type()};
        }

        for (const auto &expr : func.body()) {
            TRY_VOID(getSymbolsFromExpression(expr.get()));
            TRY(typeCheckExpr(func.name(), expr.get()));
        }
    }

    return {};
}



[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArg(const Var &arg) const {
    return fmt::format("(param ${} {})", arg.name(), arg.type().name());
}


[[nodiscard]] ErrorOr<std::string> WasmCompiler::compileArgs(const std::vector<Var> &args) const {
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
        return fmt::format("{}\n(local.set ${})\n",
            TRY(compileEpxr(funcName, declare->source().get())),
            declare->target().name());
    }
    if (const auto *ret = dynamic_cast<const Return *>(expr)) {
        return fmt::format("{}\nreturn\n", TRY(compileEpxr(funcName, ret->value().get())));
    }
    if (const auto *add = dynamic_cast<const Add *>(expr)) {
        // Need to know the type of expresision
        return fmt::format("{}\n{}\ni32.add",
            TRY(compileEpxr(funcName, add->lhs().get())),
            TRY(compileEpxr(funcName, add->rhs().get())));
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
            fmt::format_to(std::back_inserter(out), "(local ${} {})\n", name, symbol.type.name());
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
        ")\n"
        ,
        func.name(),
        func.ret().name(),
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

ErrorOr<std::string> WasmCompiler::compile(Program &&program) const {
    return compileModule(program.mod());
}
