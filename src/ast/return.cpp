#include "return.hpp"

#include "ftrace.hpp"

#include <utility>  // for move

namespace Ast {

Return::Return(ExpressionPtr argument)
        : m_argument(std::move(argument)) {
    ftrace();
}

String Return::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}
