#include "ast.hpp"

#include <iomanip>
#include <iostream>

Ident::Ident(std::string name)
        : m_name(std::move(name)) { }
Ident::Ident(std::string_view name)
        : m_name(name) { }
Ident::Ident(const char *name)
        : m_name(name) { }

Ident::operator std::string_view() const {
    return m_name;
}

Arg::Arg(Ident name, Type type)
        : m_name(std::move(name))
        , m_type(type) { }

Assign::Assign(Ident target, std::shared_ptr<Expr> source)
        : m_target(std::move(target))
        , m_source(std::move(source)) { }

Declare::Declare(Mod mod, Ident name, std::optional<Type> declaredType, std::shared_ptr<Expr> source)
        : m_mod(mod)
        , m_name(std::move(name))
        , m_declaredType(std::move(declaredType))
        , m_source(std::move(source)) { }

Return::Return(std::shared_ptr<Expr> value)
        : m_value(std::move(value)) { }

If::If(std::shared_ptr<Expr> conditon, std::vector<std::shared_ptr<Expr>> body)
        : m_conditon(std::move(conditon))
        , m_body(std::move(body)) { }

const Expr *If::conditon() const {
    return m_conditon.get();
}
const std::vector<std::shared_ptr<Expr>> *If::ifBody() const {
    return &m_body;
}
const std::vector<std::shared_ptr<Expr>> *If::elseBody() const {
    return nullptr;
}

IfElse::IfElse(If if_, std::vector<std::shared_ptr<Expr>> elseBody)
        : m_if(std::move(if_))
        , m_elseBody(std::move(elseBody)) { }

const Expr *IfElse::conditon() const {
    return m_if.conditon();
}
const std::vector<std::shared_ptr<Expr>> *IfElse::ifBody() const {
    return m_if.ifBody();
}
const std::vector<std::shared_ptr<Expr>> *IfElse::elseBody() const {
    return &m_elseBody;
}

Add::Add(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
        : m_lhs(std::move(lhs))
        , m_rhs(std::move(rhs)) { }

VarExpr::VarExpr(Ident name)
        : m_name(std::move(name)) { }

Bool::Bool(bool value)
        : m_value(value) { }

Int32::Int32(int64_t value)
        : m_value(static_cast<int32_t>(value)) { }

Int64::Int64(int64_t value)
        : m_value(value) { }

Float32::Float32(double value)
        : m_value(static_cast<float>(value)) { }

Float64::Float64(double value)
        : m_value(value) { }

Type Bool::toType() const {
    return Type::fromBuiltinType(BuiltInType::boolean);
}

Type Int32::toType() const {
    return Type::fromBuiltinType(BuiltInType::i32);
}
Type Int64::toType() const {
    return Type::fromBuiltinType(BuiltInType::i64);
}

Type Float32::toType() const {
    return Type::fromBuiltinType(BuiltInType::f32);
}
Type Float64::toType() const {
    return Type::fromBuiltinType(BuiltInType::f64);
}



FunctionDef::FunctionDef(Ident name, Type ret, std::vector<Arg> args, std::vector<std::shared_ptr<Expr>> body)
        : m_name(std::move(name))
        , m_ret(std::move(ret))
        , m_args(std::move(args))
        , m_body(std::move(body)) { }

Module::Module(Ident name, std::vector<FunctionDef> funcs)
        : m_name(std::move(name))
        , m_funcs(std::move(funcs)) { }

Program::Program(Module mod)
        : m_mod(std::move(mod)) { }
