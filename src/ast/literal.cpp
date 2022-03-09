#include "literal.hpp"

#include "ftrace.hpp"

namespace Ast {

Literal::Literal(Value value)
        : m_value(value) {
    ftrace();
}

String Literal::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);

    if (m_value.type() != comp.funcs().back().ret) {
        crash("mismathced return types. trying to return {} from a function expecting {}",
            m_value.type(),
            comp.funcs().back().ret);
    }
    switch (m_value.type()) {
        // clang-format off
        case ValueType::I32:
            return fmt::format("(i32.const {})", m_value.as_i32());
        case ValueType::I64:
            return fmt::format("(i64.const {})", m_value.as_i64());
        case ValueType::F32:
            return fmt::format("(f32.const {})", m_value.as_f32());
        case ValueType::F64:
            return fmt::format("(f64.const {})", m_value.as_f64());
        BAD_ENUM_CASE(ValueType::TYPE)
            // clang-format on
    }
    unreachable("All types have been handleled");
}

}  // namespace Ast
