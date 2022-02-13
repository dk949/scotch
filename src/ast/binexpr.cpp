#include "binexpr.hpp"

#include "ftrace.hpp"
#include "types.hpp"

#include <utility>


Ast::BinExpr::BinExpr(BinOp op, ExpressionPtr lhs, ExpressionPtr rhs)
        : m_op(op)
        , m_lhs(std::move(lhs))
        , m_rhs(std::move(rhs)) {
    ftrace();
}
