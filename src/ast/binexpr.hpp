#ifndef BINEXPR_HPP
#define BINEXPR_HPP
#include "ast/node.hpp"
#include "class_tools.hpp"
#include "compiler/compiler.hpp"
#include "expression.hpp"
#include "types.hpp"

#include <fmt/format.h>

namespace Ast {

class BinExpr : public Expression {
private:
    BinOp m_op;
    ExpressionPtr m_lhs;
    ExpressionPtr m_rhs;

public:
    explicit BinExpr(BinOp op, ExpressionPtr lhs, ExpressionPtr rhs);

    GetProperty(op);
    GetProperty(lhs);
    GetProperty(rhs);

    virtual String compile(Comp::Compiler &) override;

private:
    addClassName(BinExpr);
};

}  // namespace Ast

#endif  // BINEXPR_HPP
