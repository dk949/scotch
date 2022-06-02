#ifndef WASM_COMPILER_HPP
#define WASM_COMPILER_HPP

#include "compiler.hpp"

#include <unordered_map>
class WasmCompiler : public Compiler {
private:
    enum class SymbolKind {
        Module,
        Function,
        Type,
        Argument,
        Variable,
    };

    static inline Type emptyType = Type::fromUnknownType();
    struct Symbol {
        SymbolKind kind;
        Type type;
    };
    std::unordered_map<Ident, Symbol> m_symbolTable;
    std::unordered_map<Ident, Mod> m_modTable;

    std::string_view m_currentFunc;

    [[nodiscard]] ErrorOr<void> typeCheckExpr(Expr *);
    [[nodiscard]] ErrorOr<Type> getType(Expr *);
    [[nodiscard]] ErrorOr<void> checkFunctionInTable(const FunctionDef &);

    [[nodiscard]] ErrorOr<std::string> compileModule(const Module &) const;
    [[nodiscard]] ErrorOr<std::string> compileFuncs(const std::vector<FunctionDef> &) const;
    [[nodiscard]] ErrorOr<std::string> compileFunc(const FunctionDef &) const;
    [[nodiscard]] ErrorOr<std::string> compileArgs(const std::vector<Arg> &) const;
    [[nodiscard]] ErrorOr<std::string> compileArg(const Arg &) const;
    [[nodiscard]] ErrorOr<std::string> compileBlock(const std::vector<std::shared_ptr<Expr>> &) const;
    [[nodiscard]] ErrorOr<std::string> compileEpxr(const Expr *) const;
    [[nodiscard]] ErrorOr<std::string> compileVars() const;

    template<typename T>
    ErrorOr<std::string> compileVec(const std::vector<T> &vec, auto &&fn) {
        std::string out;
        for (const auto &v : vec) {
            fmt::format_to(std::back_inserter(out), "{}", TRY(fn(v)));
        }
        return out;
    };

public:
    [[nodiscard]] ErrorOr<void> typeCheck() override;
    [[nodiscard]] ErrorOr<std::string> compile() override;
    [[nodiscard]] constexpr std::string_view compilerType() const override {
        return "WasmCompiler";
    }
};


#endif  // WASM_COMPILER_HPP
