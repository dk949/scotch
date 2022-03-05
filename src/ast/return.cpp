#include "return.hpp"

#include "binexpr.hpp"
#include "call.hpp"
#include "common.hpp"
#include "expressionstatement.hpp"
#include "ftrace.hpp"
#include "literal.hpp"

#include <utility>  // for move

namespace Ast {

Return::Return(ExpressionPtr argument)
        : m_argument(std::move(argument)) {
    ftrace();
}

String Return::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    String out;
    if (auto *lit = is<Literal>(m_argument)) {
        out.append(lit->compile(comp));
    } else {
        unreachable("Unexpected Expression type '{}'", to<NodePtr>(m_argument)->className());
    }

    return out;
}

}  // namespace Ast
