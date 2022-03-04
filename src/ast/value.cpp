#include "value.hpp"

#include "ftrace.hpp"
#include "log.hpp"

namespace Ast {

Value::Value(ValueType type)
        : m_type(type) {
    ftrace();
}

Value::Value(Int64 val)
        : m_type(ValueType::INT) {
    ftrace();
    m_value.as_int = val;
}

Int64 Value::as_int() {
    ftrace();
    verify(m_type == ValueType::INT);
    return m_value.as_int;
}

ValueType Value::as_type() {
    ftrace();
    verify(m_type == ValueType::TYPE);
    return m_value.as_type;
}

String Value::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}  // namespace Ast
