#include "value.hpp"

#include "ftrace.hpp"
#include "log.hpp"

Ast::Value::Value(ValueType type)
        : m_type(type) {
    ftrace();
}

Ast::Value::Value(Int64 val)
        : m_type(ValueType::INT) {
    ftrace();
    m_value.as_int = val;
}

Int64 Ast::Value::as_int() {
    ftrace();
    verify(m_type == ValueType::INT);
    return m_value.as_int;
}

Ast::ValueType Ast::Value::as_type() {
    ftrace();
    verify(m_type == ValueType::TYPE);
    return m_value.as_type;
}
