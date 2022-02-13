#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "node.hpp"
namespace Ast {
class Expression : public Node {
public:
    friend String Ast::printNode(const NodePtr &node);
private:
    addClassName(Expression);
};
}  // namespace Ast

#endif  // EXPRESSION_HPP
