#ifndef COMPILER_HPP
#define COMPILER_HPP


#include "ast.hpp"
#include "error.hpp"

#include <string_view>

class Compiler {
protected:
    Program m_program;

public:
    Compiler(Program);
    [[nodiscard]] virtual ErrorOr<void> typeCheck() = 0;
    [[nodiscard]] virtual ErrorOr<std::string> compile() = 0;
    [[nodiscard]] virtual constexpr std::string_view compilerType() const = 0;
    virtual ~Compiler() = default;
};

#endif  // COMPILER_HPP
