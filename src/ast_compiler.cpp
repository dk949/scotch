#include "ast_compiler.hpp"

#include "ast.hpp"

#include <fmt/format.h>

ErrorOr<std::string> compileType(const Type &type) {
    return fmt::format("Type({}, {})", type.mod() == Mod::CONST ? "const" : "let", type.name().name());
}

ErrorOr<std::string> compileVar(const Var &var) {
    return fmt::format("Var({}, {})", TRY(compileType(var.type())), var.name().name());
}

ErrorOr<std::string> compileExpression(const std::shared_ptr<Expr> expr, std::string_view pre = "  ", std::string_view post = ",\n") {
    std::string out {pre};
    if (Assign *assign = dynamic_cast<Assign *>(expr.get())) {
        fmt::format_to(std::back_inserter(out),
            "Assign({}, {})",
            assign->target().name(),
            TRY(compileExpression(assign->source(), "", "")));

    } else if (Declare *declare = dynamic_cast<Declare *>(expr.get())) {
        fmt::format_to(std::back_inserter(out),
            "Declare({}, {})",
            TRY(compileVar(declare->target())),
            TRY(compileExpression(declare->source(), "", "")));

    } else if (Return *ret = dynamic_cast<Return *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Return({})", TRY(compileExpression(ret->value(), "", "")));

    } else if (Add *add = dynamic_cast<Add *>(expr.get())) {
        fmt::format_to(std::back_inserter(out),
            "Add({}, {})",
            TRY(compileExpression(add->lhs(), "", "")),
            TRY(compileExpression(add->rhs(), "", "")));
    } else if (VarExpr *varexpr = dynamic_cast<VarExpr *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Access({})", varexpr->name().name());
    } else if (Int32 *int32 = dynamic_cast<Int32 *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Int32({})", int32->value());
    } else if (Int64 *int64 = dynamic_cast<Int64 *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Int64({})", int64->value());
    } else if (Float32 *float32 = dynamic_cast<Float32 *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Float32({})", float32->value());
    } else if (Float64 *float64 = dynamic_cast<Float64 *>(expr.get())) {
        fmt::format_to(std::back_inserter(out), "Float64({})", float64->value());
    } else {
        return tl::unexpected(Error {fmt::format("Unexpected expression type {}", expr->exprType())});
    }
    fmt::format_to(std::back_inserter(out), "{}", post);
    return out;
}

ErrorOr<std::string> compileFunc(const FunctionDef &func) {
    auto out = fmt::format("Function({}, Args(", func.name().name());
    int notFirst = 0;
    for (const auto &var : func.args()) {
        fmt::format_to(std::back_inserter(out), "{}{}", notFirst++ ? ", " : "", TRY(compileVar(var)));
    }

    fmt::format_to(std::back_inserter(out), "), {}, Body(", TRY(compileType(func.ret())));

    for (const auto &expr : func.body()) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileExpression(expr)));
    }
    fmt::format_to(std::back_inserter(out), "))\n");
    return out;
}

ErrorOr<std::string> compileModule(const Module &mod) {
    auto out = fmt::format("Module({})\n", mod.name().name());

    const auto &funcs = mod.funcs();
    for (const auto &func : funcs) {
        fmt::format_to(std::back_inserter(out), "{}", TRY(compileFunc(func)));
    }
    return out;
}

ErrorOr<std::string> AstCompiler::compile(Program &&program) const {
    return compileModule(program.mod());
}
