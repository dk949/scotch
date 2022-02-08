#ifndef RETURN_HPP
#define RETURN_HPP
#include "expression.hpp"

namespace Ast {
class Return : public Node {
private:
    ExpressionPtr m_argument;

public:
    explicit Return(ExpressionPtr argument);

    const inline ExpressionPtr argument() const {
        return m_argument;
    }

private:
    addClassName(Return);
};
}  // namespace Ast

#endif  // RETURN_HPP
