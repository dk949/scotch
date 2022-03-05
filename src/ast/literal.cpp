#include "literal.hpp"

#include "ftrace.hpp"

namespace Ast {

Literal::Literal(Value value)
        : m_value(value) {
    ftrace();
}

String Literal::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);

    switch (m_value.type()) {
        // clang-format off
        case ValueType::INT:
            return fmt::format("(i64.const {})", m_value.as_int());
        BAD_ENUM_CASE(ValueType::TYPE)
        default:
            unreachable("Handeled all value types");

            // clang-format on
    }
}

}  // namespace Ast
