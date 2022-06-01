#include "compiler.hpp"

void Compiler::setProgram(Program p) {
    m_program = std::move(p);
}
void Compiler::setTypeStore(TypeStore ts) {
    m_ts = std::move(ts);
}
