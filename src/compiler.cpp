#include "compiler.hpp"
Compiler::Compiler(Program p)
        : m_program(std::move(p)) { }
