#ifndef CALL_HPP
#define CALL_HPP

#include "expression.hpp"
namespace Ast {

class Call : public Expression {
    public:
    friend String Ast::printNode(NodePtr node);
private:
    addClassName(Call);
};
}  // namespace Ast

#endif  // CALL_HPP
