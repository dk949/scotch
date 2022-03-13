#include "module.hpp"


namespace Ast {

Module::Module(String name)
        : m_name {std::move(name)} { }

String Module::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    String out = fmt::format("(module ${}", m_name);
    for (auto &child : m_children) {
        out.append(child->compile(comp));
    }
    out.push_back(')');
    return out;
}

}  // namespace Ast
