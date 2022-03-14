#include "functiondecl.hpp"

#include "common.hpp"
#include "log.hpp"
#include "scope.hpp"

#include <utility>  // for move

namespace Ast {

FunctionDecl::FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body)
        : m_name(std::move(name))
        , m_args(std::move(args))
        , m_ret(ret)
        , m_body(std::move(body)) {
    ftrace();
}

String FunctionDecl::compile(Comp::Compiler &comp) {
    ftrace();
    String pre = fmt::format("(func ${0} (export \"{0}\")", m_name);
    comp.appendFunc(m_name, m_args, m_ret);
    if (!m_args.empty()) {
        crash("Funciton arguments not yet supported");
    }

    // Declaring return type of the funciton and the local into which this return value will be written
    fmt::format_to(std::back_inserter(pre), "(result {0})(local {0})", m_ret);

    String out = fmt::format("(block ${}", m_name);
    for (auto &child : m_body->children()) {
        out.append(child->compile(comp));
    }
    out.append(")(local.get 0))");

    for (const auto &local : comp.funcs().back().locals) {
        pre.append(local.decl);
    }

    return pre + out;
}

}  // namespace Ast
