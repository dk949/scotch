#include "value.hpp"

Ast::Value::Value(ValueType type)
        : m_type(type) {
    trace();
}

Ast::Value::Value(Int64 val)
        : m_type(ValueType::INT) {
    trace();
    m_value.as_int = val;
}

Int64 Ast::Value::as_int() {
    trace();
    verify(m_type == ValueType::INT);
    return m_value.as_int;
}

Ast::ValueType Ast::Value::as_type() {
    trace();
    verify(m_type == ValueType::TYPE);
    return m_value.as_type;
}
