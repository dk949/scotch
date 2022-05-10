#include "ast_compiler.hpp"
#include <iostream>

void printType(const Type &type) {
    std::cout << "Type(" << (type.mod() == Mod::CONST ? "const" : "let") << ", " << type.name().name() << ')';
}

void printVar(const Var &var) {
    std::cout << "Var(";
    printType(var.type());
    std::cout << ", ";
    std::cout << var.name().name();
    std::cout << ')';
}

void printExpression(const std::shared_ptr<Expr> expr, std::string_view pre = "  ", std::string_view post = ",\n") {
    if (Assign *assign = dynamic_cast<Assign *>(expr.get())) {
        std::cout << pre << "Assign(" << assign->target().name() << ", ";
        printExpression(assign->source(), "", "");
        std::cout << ')';

    } else if (Declare *declare = dynamic_cast<Declare *>(expr.get())) {
        std::cout << pre << "Declare(";
        printVar(declare->target());
        std::cout << ", ";
        printExpression(declare->source(), "", "");
        std::cout << ')';

    } else if (Return *ret = dynamic_cast<Return *>(expr.get())) {
        std::cout << pre << "Return(";
        printExpression(ret->value(), "", "");
        std::cout << ')';

    } else if (Add *add = dynamic_cast<Add *>(expr.get())) {
        std::cout << "Add(";
        printExpression(add->lhs(), "", "");
        std::cout << ", ";
        printExpression(add->rhs(), "", "");
        std::cout << ')';

    } else if (VarExpr *varexpr = dynamic_cast<VarExpr *>(expr.get())) {
        std::cout << "Access(" << varexpr->name().name() << ')';

    } else if (Int32 *int32 = dynamic_cast<Int32 *>(expr.get())) {
        std::cout << "Int32(" << int32->value() << ")";

    } else if (Int64 *int64 = dynamic_cast<Int64 *>(expr.get())) {
        std::cout << "Int64(" << int64->value() << ")";

    } else if (Float32 *float32 = dynamic_cast<Float32 *>(expr.get())) {
        std::cout << "Float32(" << float32->value() << ")";

    } else if (Float64 *float64 = dynamic_cast<Float64 *>(expr.get())) {
        std::cout << "Float64(" << float64->value() << ")";
    }
    std::cout << post;
}

void printFunc(const FunctionDef &func) {
    std::cout << "Function(" << func.name().name() << ", Args(";
    int notFirst = 0;
    for (const auto &var : func.args()) {
        std::cout << (notFirst++ ? ", " : "");
        printVar(var);
    }
    std::cout << "), ";
    printType(func.ret());
    std::cout << ", Body(\n";
    for (const auto &expr : func.body()) {
        printExpression(expr);
    }
    std::cout << "))\n";
}

void printModule(const Module &mod) {
    std::cout << "Module(" << mod.name().name() << ")\n";

    const auto funcs = mod.funcs();
    for (const auto &func : funcs) {
        printFunc(func);
    }
}

void printProgram(const Program &program) {
    printModule(program.mod());
}
