#ifndef AST_HPP
#define AST_HPP
#include "ast_fwd.hpp"
#include "macros.hpp"

#include <memory>
#include <string>
#include <vector>

enum class Mod { CONST, LET };

class Ident {
    std::string PropertyGet(name)
public:
    Ident(std::string name);
    Ident(std::string_view name);
    Ident(const char *name);
    Ident() = default;
};

class Type {
    Mod PropertyGet(mod)
    Ident PropertyGet(name)
public:
    Type(Mod mod, Ident name);
    Type() = default;
};

class Var {
    Ident PropertyGet(name)
    Type PropertyGet(type)
public:
    Var(Ident name, Type type);
    Var() = default;
};


struct Expr {
    virtual ~Expr() = default;
    virtual constexpr std::string_view exprType() const = 0;
};

class Assign : public Expr {
    Ident PropertyGet(target)
    std::shared_ptr<Expr> PropertyGet(source)
public:
    Assign(Ident target, std::shared_ptr<Expr> source);
    Assign() = default;
    std::string_view constexpr exprType() const override {
        return "Assign";
    }
};

class Declare : public Expr {
    Var PropertyGet(target)
    std::shared_ptr<Expr> PropertyGet(source)
public:
    Declare(Var target, std::shared_ptr<Expr> source);
    Declare() = default;

    std::string_view constexpr exprType() const override {
        return "Declare";
    }
};

class Return : public Expr {
    std::shared_ptr<Expr> PropertyGet(value)
public:
    Return(std::shared_ptr<Expr> value);
    Return() = default;
    std::string_view constexpr exprType() const override {
        return "Return";
    }
};

class Add : public Expr {
    std::shared_ptr<Expr> PropertyGet(lhs)
    std::shared_ptr<Expr> PropertyGet(rhs)
public:
    Add(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    Add() = default;
    std::string_view constexpr exprType() const override {
        return "Add";
    }
};

class VarExpr : public Expr {
    Ident PropertyGet(name)
public:
    VarExpr(Ident name);
    VarExpr() = default;
    std::string_view constexpr exprType() const override {
        return "VarExpr";
    }
};

class Int32 : public Expr {
    int32_t PropertyGet(value)
public:
    Int32(int64_t value);
    Int32() = default;
    std::string_view constexpr exprType() const override {
        return "Int32";
    }
};

class Int64 : public Expr {
    int64_t PropertyGet(value)
public:
    Int64(int64_t value);
    Int64() = default;
    std::string_view constexpr exprType() const override {
        return "Int64";
    }
};

struct Float32 : public Expr {
    float PropertyGet(value)
public:
    Float32(double value);
    Float32() = default;
    std::string_view constexpr exprType() const override {
        return "Float32";
    }
};
struct Float64 : public Expr {
    double PropertyGet(value)
public:
    Float64(double value);
    Float64() = default;
    std::string_view constexpr exprType() const override {
        return "Float64";
    }
};

class EmptyExpr : public Expr {
    std::string_view constexpr exprType() const override {
        return "EmptyExpr";
    }
};

class FunctionDef {
    Ident PropertyGet(name)
    Type PropertyGet(ret)
    std::vector<Var> PropertyGet(args)
    std::vector<std::shared_ptr<Expr>> PropertyGet(body)
public:
    FunctionDef(Ident name, Type ret, std::vector<Var> args, std::vector<std::shared_ptr<Expr>> body);
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

#endif  // AST_HPP
