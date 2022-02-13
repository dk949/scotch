#ifndef COMPILER_HPP
#define COMPILER_HPP
#include "types.hpp"

namespace Comp {

class Compiler {
private:
    Ast::ProgramPtr m_prog;
public:
    explicit Compiler(Ast::ProgramPtr);

    String compile();
};

}

#endif  // COMPILER_HPP
