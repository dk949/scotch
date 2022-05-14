#ifndef AST_COMPILER_HPP
#define AST_COMPILER_HPP
#include "ast_fwd.hpp"
#include "compiler.hpp"


class AstCompiler : public Compiler {
    [[nodiscard]] ErrorOr<void> typeCheck(const Program &) override {
        return {};
    }

    [[nodiscard]] ErrorOr<std::string> compile(Program &&) override;

    [[nodiscard]] constexpr std::string_view compilerType() const override {
        return "AstCompiler";
    }
};

void printProgram(const Program &);

#endif  // AST_COMPILER_HPP
