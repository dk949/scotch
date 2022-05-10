#ifndef AST_HPP
#define AST_HPP
#include "macros.hpp"

#include <memory>
#include <string>
#include <vector>

enum class Mod { CONST, LET };

class Ident {
    std::string PropertyGet(name) public : Ident(std::string name);
    Ident() = default;
};

class Type {
    Mod PropertyGet(mod)
    Ident PropertyGet(name) public : Type(Mod mod, Ident name);
    Type() = default;
};

class Var {
    Ident PropertyGet(name)
    Type PropertyGet(type) public : Var(Ident name, Type type);
    Var() = default;
};



struct Expr {
    virtual ~Expr() = default;
};

class Assign : public Expr {
    Ident PropertyGet(target) std::shared_ptr<Expr> PropertyGet(source) public : Assign(Ident target, std::shared_ptr<Expr> source);
    Assign() = default;
};

class Declare : public Expr {
    Var PropertyGet(target) std::shared_ptr<Expr> PropertyGet(source) public : Declare(Var target, std::shared_ptr<Expr> source);
    Declare() = default;
};

class Return : public Expr {
    std::shared_ptr<Expr> PropertyGet(value) public : Return(std::shared_ptr<Expr> value);
    Return() = default;
};

class Add : public Expr {
    std::shared_ptr<Expr> PropertyGet(lhs) std::shared_ptr<Expr> PropertyGet(rhs) public
            : Add(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    Add() = default;
};

class VarExpr : public Expr {
    Ident PropertyGet(name) public : VarExpr(Ident name);
    VarExpr() = default;
};

class Int32 : public Expr {
    int32_t PropertyGet(value) public : Int32(int64_t value);
    Int32() = default;
};

class Int64 : public Expr {
    int64_t PropertyGet(value) public : Int64(int64_t value);
    Int64() = default;
};

struct Float32 : public Expr {
    float PropertyGet(value) public : Float32(double value);
    Float32() = default;
};
struct Float64 : public Expr {
    double PropertyGet(value) public : Float64(double value);
    Float64() = default;
};

class EmptyExpr : public Expr { };

class FunctionDef {
    Ident PropertyGet(name)
    Type PropertyGet(ret) std::vector<Var> PropertyGet(args) std::vector<std::shared_ptr<Expr>> PropertyGet(body) public
            : FunctionDef(Ident name, Type ret, std::vector<Var> args, std::vector<std::shared_ptr<Expr>> body);
    FunctionDef() = default;
};

class Module {
    Ident PropertyGet(name) std::vector<FunctionDef> PropertyGet(funcs) public
            : Module(Ident name, std::vector<FunctionDef> funcs);
    Module() = default;
};

class Program {
    Module PropertyGet(mod) public : Program(Module mod);
    Program() = default;
};

#endif  // AST_HPP
