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
    ftrace();
    String pre = fmt::format("(func ${0} (export \"{0}\")", m_name);
    comp.appendFunc(m_name, m_args, m_return);
    if (!m_args.empty()) {
        crash("Funciton arguments not yet supported");
    }

    pre.append("(result ");
    switch (m_return) {
        case ValueType::I32:
            pre.append("i32)");
            break;
        case ValueType::I64:
            pre.append("i64)");
            break;
        case ValueType::F32:
            pre.append("f32)");
            break;
        case ValueType::F64:
            pre.append("f64)");
            break;
        default:
            crash("Only int return is supported for now");
    }
    String out;

    for (auto &child : m_body->children()) {
        out.append(child->compile(comp));
    }
    out.push_back(')');

    for (const auto &local : comp.funcs().back().locals) {
        pre.append(local.decl);
    }

    return pre + out;
}

}  // namespace Ast
