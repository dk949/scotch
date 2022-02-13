#ifndef LITERAL_HPP
#define LITERAL_HPP

#include "ast/node.hpp"
#include "ast/value.hpp"
#include "expression.hpp"
#include "types.hpp"

namespace Ast {
class Literal : public Expression {

private:
    Value m_value;
public:
    Literal(Value value);

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Literal);
};

}  // namespace Ast
#endif  // LITERAL_HPP
