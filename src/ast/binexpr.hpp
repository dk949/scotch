#ifndef BINEXPR_HPP
#define BINEXPR_HPP
#include "ast/node.hpp"    // for addClassName
#include "expression.hpp"  // for Expression
#include "types.hpp"       // for ExpressionPtr, BinOp, StringView

#include <fmt/format.h>

namespace Ast {

class BinExpr : public Expression {
private:
    BinOp m_op;
    ExpressionPtr m_lhs;
    ExpressionPtr m_rhs;

public:
    explicit BinExpr(BinOp op, ExpressionPtr lhs, ExpressionPtr rhs);

    friend String Ast::printNode(NodePtr node);

private:
    addClassName(BinExpr);
};

}

#endif  // BINEXPR_HPP
