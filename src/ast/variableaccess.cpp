#include "variableaccess.hpp"

#include "common.hpp"
#include "compiler/compiler.hpp"

namespace Ast {

VariableAccess::VariableAccess(String name)
        : m_name(std::move(name)) {
    ftrace();
}

String VariableAccess::compile(Comp::Compiler &comp) {
    if (!Tools::contains<Vector<Comp::LocalRep>, StringView>(comp.funcs().back().locals, StringView {m_name})) {
        crash("No such variable {}", m_name);
    }
    return fmt::format("(local.get ${})", m_name);
}

}
