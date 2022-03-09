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
    comp.appendFunc(m_name, m_args, m_return);
    if (!m_args.empty()) {
        crash("Funciton arguments not yet tupported");
    }

    out.append("(result ");
    switch (m_return) {
        case ValueType::I32:
            out.append("i32)");
            break;
        case ValueType::I64:
            out.append("i64)");
            break;
        case ValueType::F32:
            out.append("f32)");
            break;
        case ValueType::F64:
            out.append("f64)");
            break;
        default:
            crash("Only int return is supported for now");
    }

    for (auto &child : m_body->children()) {
        out.append(child->compile(comp));
    }

    out.push_back(')');
    return out;
}

}  // namespace Ast
