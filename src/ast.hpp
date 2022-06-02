#ifndef AST_HPP
#define AST_HPP
#include "type.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

enum class Mod { NONE, CONST, LET };

class Ident {
    std::string PropertyGet(name)
public:
    Ident(std::string name);
    Ident(std::string_view name);
    Ident(const char *name);
    Ident() = default;
    operator std::string_view() const;

    bool operator==(const Ident &) const = default;
};

template<>
struct std::hash<Ident> {
    std::size_t operator()(const Ident &i) const noexcept {
        return std::hash<std::string> {}(i.name());
    }
};

class Arg {
    Ident PropertyGet(name)
    Type PropertyGet(type)
public:
    Arg(Ident name, Type type);
    Arg() = default;
};


class Expr {
    std::optional<Type> PropertyGetSet(overallType)
public:
    virtual ~Expr() = default;
    virtual constexpr std::string_view exprType() const = 0;
};

class Assign : public Expr {
    Ident PropertyGet(target)
    std::shared_ptr<Expr> PropertyGet(source)
public:
    Assign(Ident target, std::shared_ptr<Expr> source);
    Assign() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Assign";
    }
};

class Declare : public Expr {
    Mod PropertyGet(mod)
    Ident PropertyGet(name)
    std::optional<Type> PropertyGet(declaredType)
    std::shared_ptr<Expr> PropertyGet(source)
public:
    Declare(Mod mod, Ident name, std::optional<Type> declaredType, std::shared_ptr<Expr> source);

    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Declare";
    }
};

class Return : public Expr {
    std::shared_ptr<Expr> PropertyGet(value)
public:
    Return(std::shared_ptr<Expr> value);
    Return() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Return";
    }
};

class Add : public Expr {
    std::shared_ptr<Expr> PropertyGet(lhs)
    std::shared_ptr<Expr> PropertyGet(rhs)
public:
    Add(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    Add() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Add";
    }
};

class VarExpr : public Expr {
    Ident PropertyGet(name)
public:
    VarExpr(Ident name);
    VarExpr() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "VarExpr";
    }
};

class BuiltInLiteral : public Expr {
public:
    [[nodiscard]] virtual Type toType() const = 0;
};

class Int32 : public BuiltInLiteral {
    int32_t PropertyGet(value)
public:
    Int32(int64_t value);
    Int32() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Int32";
    }
    [[nodiscard]] Type toType() const override;
};

class Int64 : public BuiltInLiteral {
    int64_t PropertyGet(value)
public:
    Int64(int64_t value);
    Int64() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Int64";
    }
    [[nodiscard]] Type toType() const override;
};

class Float32 : public BuiltInLiteral {
    float PropertyGet(value)
public:
    Float32(double value);
    Float32() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Float32";
    }
    [[nodiscard]] Type toType() const override;
};
class Float64 : public BuiltInLiteral {
    double PropertyGet(value)
public:
    Float64(double value);
    Float64() = default;
    [[nodiscard]] std::string_view constexpr exprType() const override {
        return "Float64";
    }
    [[nodiscard]] Type toType() const override;
};

class EmptyExpr : public Expr {
    std::string_view constexpr exprType() const override {
        return "EmptyExpr";
    }
};

class FunctionDef {
    Ident PropertyGet(name)
    Type PropertyGet(ret)
    std::vector<Arg> PropertyGet(args)
    std::vector<std::shared_ptr<Expr>> PropertyGet(body)
public:
    FunctionDef(Ident name, Type ret, std::vector<Arg> args, std::vector<std::shared_ptr<Expr>> body);
    FunctionDef() = default;
};

class Module {
    Ident PropertyGet(name)
    std::vector<FunctionDef> PropertyGet(funcs)
public:
    Module(Ident name, std::vector<FunctionDef> funcs);
    Module() = default;
};

class Program {
    Module PropertyGet(mod)
public:
    Program(Module mod);
    Program() = default;
};

// Printing

template<>
struct fmt::formatter<Ident> : formatter<std::string> {
    auto format(const Ident &id, format_context &ctx) {
        return formatter<std::string>::format(id.name(), ctx);
    }
};

template<>
struct fmt::formatter<Mod> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(Mod mod, FormatContext &ctx) {
        std::string name = "Invalid token";
        switch (mod) {
            case Mod::NONE:
                name = "NONE";
                break;
            case Mod::CONST:
                name = "CONST";
                break;
            case Mod::LET:
                name = "LET";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};


#endif  // AST_HPP
