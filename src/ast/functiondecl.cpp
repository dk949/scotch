#include "functiondecl.hpp"

#include "common.hpp"
#include "log.hpp"
#include "scope.hpp"

#include <utility>  // for move

namespace Ast {

FunctionDecl::FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body)
        : m_name(std::move(name))
        , m_args(std::move(args))
        , m_return(ret)
        , m_body(std::move(body)) {
    ftrace();
}

String FunctionDecl::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    String out = fmt::format("(func ${0} (export \"{0}\")", m_name);
    comp.appendFunc(m_name);
    if (!m_args.empty()) {
        crash("Funciton arguments not yet tupported");
    }

    out.append("(result ");
    if (m_return == ValueType::INT) {
        out.append("i64)");
    } else {
        crash("Only int return is supported for now");
    }

    for (auto &child : m_body->children()) {
        out.append(child->compile(comp));
    }

    out.push_back(')');
    return out;
}

}  // namespace Ast
