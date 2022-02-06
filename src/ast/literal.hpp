#ifndef LITERAL_HPP
#define LITERAL_HPP

#include "expression.hpp"

namespace Ast {
class Literal : public Expression {

private:
    Value m_value;
public:
    Literal(Value value)
            : m_value(value) { }


private:
    addClassName(Literal);
};

}  // namespace Ast
#endif  // LITERAL_HPP
