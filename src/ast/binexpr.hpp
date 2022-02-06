#ifndef BINEXPR_HPP
#define BINEXPR_HPP
#include "expression.hpp"
#include "types.hpp"

namespace Ast {
class BinExpr : public Expression {
private:
    BinOp m_op;
    ExpressionPtr m_lhs;
    ExpressionPtr m_rhs;

public:
    explicit BinExpr(BinOp op, ExpressionPtr lhs, ExpressionPtr rhs)
            : m_op(std::move(op))
            , m_lhs(std::move(lhs))
            , m_rhs(std::move(rhs)) { }

private:
    addClassName(BinExpr);
};
}  // namespace Ast

#endif  // BINEXPR_HPP
