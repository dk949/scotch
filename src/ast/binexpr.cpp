#include "binexpr.hpp"
Ast::BinExpr::BinExpr(BinOp op, ExpressionPtr lhs, ExpressionPtr rhs)
        : m_op(std::move(op))
        , m_lhs(std::move(lhs))
        , m_rhs(std::move(rhs)) { }
