#include "program.hpp"

#include "ftrace.hpp"


namespace Ast {

String Program::compile(Comp::Compiler &comp) {
    ftrace();
    spdlog::debug("Current compiler state = {}", comp);
    String out;
    for (auto &mod : m_modules) {
        comp.setModName(mod->name());
        out.append(mod->compile(comp));
    }
    return out;
}
}
