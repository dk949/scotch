#ifndef LITERAL_HPP
#define LITERAL_HPP

#include "ast/node.hpp"    // for addClassName
#include "ast/value.hpp"   // for Value
#include "expression.hpp"  // for Expression
#include "types.hpp"       // for StringView

namespace Ast {
class Literal : public Expression {

private:
    Value m_value;
public:
    Literal(Value value);

    friend String Ast::printNode(NodePtr node);

private:
    addClassName(Literal);
};

}  // namespace Ast
#endif  // LITERAL_HPP
