#ifndef RETURN_HPP
#define RETURN_HPP
#include "ast/node.hpp"
#include "compiler/compiler.hpp"
#include "types.hpp"

namespace Ast {
class Return : public Node {
private:
    ExpressionPtr m_argument;

public:
    explicit Return(ExpressionPtr argument);

    virtual String compile(Comp::Compiler &) override;

    [[nodiscard]] inline ExpressionPtr argument() const {
        return m_argument;
    }

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Return);
};
}  // namespace Ast

#endif  // RETURN_HPP
