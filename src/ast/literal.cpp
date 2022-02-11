#include "literal.hpp"
#include "ftrace.hpp"

Ast::Literal::Literal(Value value)
        : m_value(value) {
    ftrace();
}
