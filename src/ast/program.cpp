#include "program.hpp"


namespace Ast {

String Program::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    String out {"(module "};
    for (auto &child : m_children) {
        out.append(child->compile(comp));
    }
    out.push_back(')');
    return out;
}

}
