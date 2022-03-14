#include "module.hpp"

#include "ftrace.hpp"


namespace Ast {

Module::Module(String name)
        : m_name {std::move(name)} {
    ftrace();
}

String Module::compile(Comp::Compiler &comp) {
    ftrace();
    String out = fmt::format("(module ${}", m_name);
    for (auto &child : m_children) {
        out.append(child->compile(comp));
    }
    out.push_back(')');
    return out;
}

}  // namespace Ast
