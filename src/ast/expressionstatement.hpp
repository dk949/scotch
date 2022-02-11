#ifndef EXPRESSIONSTATEMENT_HPP
#define EXPRESSIONSTATEMENT_HPP

#include "expression.hpp"
namespace Ast {

class ExpressionStatement : public Expression {
public:
    friend String Ast::printNode(NodePtr node);
private:
    addClassName(Call);
};
}  // namespace Ast

#endif  // EXPRESSIONSTATEMENT_HPP
