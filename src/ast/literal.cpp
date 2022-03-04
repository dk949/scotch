#include "literal.hpp"

#include "ftrace.hpp"

namespace Ast {

Literal::Literal(Value value)
        : m_value(value) {
    ftrace();
}

String Literal::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}
