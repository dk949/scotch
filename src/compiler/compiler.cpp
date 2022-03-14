#include "compiler.hpp"

#include "ast.hpp"
#include "common.hpp"
#include "ftrace.hpp"
#include "log.hpp"

namespace Comp {

Compiler::Compiler(Ast::ProgramPtr prog)
        : m_prog(std::move(prog)) {
    ftrace();
}

String Compiler::compile() {
    return m_prog->compile(*this);
}

void Compiler::appendFunc(const FuncRep &func) {
    m_funcs.push_back(func);
}

void Compiler::appendFunc(StringView name, const Vector<Ast::ValueType> &args, Ast::ValueType ret, const std::vector<LocalRep> &locals) {
    m_funcs.emplace_back(name, args, ret, locals);
}

void Compiler::appendVar(const LocalRep &var) {
    m_funcs.back().locals.push_back(var);
}


void Compiler::setModName(StringView name) {
    m_moduleName = name;
}



}  // namespace Comp
