#include "value.hpp"

#include "ftrace.hpp"
#include "log.hpp"

namespace Ast {

Value::Value(ValueType type)
        : m_type(type) {
    ftrace();
}

Value::Value(Int32 val)
        : m_type(ValueType::I32) {
    ftrace();
    m_value.as_i32 = val;
}

Value::Value(Int64 val)
        : m_type(ValueType::I64) {
    ftrace();
    m_value.as_i64 = val;
}
Value::Value(Float32 val)
        : m_type(ValueType::F32) {
    ftrace();
    m_value.as_f32 = val;
}
Value::Value(Float64 val)
        : m_type(ValueType::F64) {
    ftrace();
    m_value.as_f64 = val;
}

Int32 Value::as_i32() {
    ftrace();
    verify(m_type == ValueType::I32);
    return m_value.as_i32;
}

Int64 Value::as_i64() {
    ftrace();
    verify(m_type == ValueType::I64);
    return m_value.as_i64;
}

Float32 Value::as_f32() {
    ftrace();
    verify(m_type == ValueType::F32);
    return m_value.as_f32;
}

Float64 Value::as_f64() {
    ftrace();
    verify(m_type == ValueType::F64);
    return m_value.as_f64;
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
