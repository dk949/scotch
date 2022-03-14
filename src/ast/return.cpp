#include "return.hpp"

#include "binexpr.hpp"
#include "call.hpp"
#include "common.hpp"
#include "expressionstatement.hpp"
#include "ftrace.hpp"
#include "literal.hpp"
#include "variableaccess.hpp"

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
        verify_msg(lit->value().type() == comp.funcs().back().ret,
            "mismathced return types. trying to return {} from a function expecting {}",
            lit->value().type(),
            comp.funcs().back().ret);
        out.append(lit->compile(comp));
    } else if (auto *var = is<VariableAccess>(m_argument)) {
        out.append(var->compile(comp));
    } else {
        unreachable("Unexpected Expression type '{}'", to<NodePtr>(m_argument)->className());
    }

    return out;
}

}  // namespace Ast
