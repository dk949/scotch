#ifndef RETURN_HPP
#define RETURN_HPP
#include "ast/node.hpp"  // for Node, addClassName
#include "types.hpp"     // for ExpressionPtr, StringView

namespace Ast {
class Return : public Node {
private:
    ExpressionPtr m_argument;

public:
    explicit Return(ExpressionPtr argument);

    const inline ExpressionPtr argument() const {
        trace();
        return m_argument;
    }

private:
    addClassName(Return);
};
}  // namespace Ast

#endif  // RETURN_HPP
