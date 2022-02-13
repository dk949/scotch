#include "compiler.hpp"

#include "ast.hpp"
#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"


Comp::Compiler::Compiler(Ast::ProgramPtr prog)
        : m_prog(std::move(prog)) {
    ftrace();
    debug("now compiling \n{}", to<Ast::NodePtr>(m_prog));
}

String Comp::Compiler::compile() {
    return "nothing yet";
}
