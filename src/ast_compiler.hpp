#ifndef AST_COMPILER_HPP
#define AST_COMPILER_HPP
#include "ast_fwd.hpp"
#include "compiler.hpp"


class AstCompiler : public Compiler {
    [[nodiscard]] virtual ErrorOr<void> typeCheck(const Program &) override {
        return {};
    }
    [[nodiscard]] virtual ErrorOr<std::string> compile(const Program &) override;
    [[nodiscard]] virtual std::string_view compilerType() const override {
        return "AstCompiler";
    }
};

void printProgram(const Program &);

#endif  // AST_COMPILER_HPP
