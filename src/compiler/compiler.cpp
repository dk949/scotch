#include "compiler.hpp"

#include "ast.hpp"
#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

namespace Comp {

Compiler::Compiler(Ast::ProgramPtr prog)
        : m_prog(std::move(prog)) {
    ftrace();
    spdlog::debug("now compiling \n{}", to<Ast::NodePtr>(m_prog));
}

String Compiler::compile() {
    return m_prog->compile(*this);
}

void Compiler::appendFunc(StringView func) {
    m_funcs.push_back(func);
}
}
