#include "compiler.hpp"

void Compiler::setProgram(Program p) {
    m_program = std::move(p);
}

