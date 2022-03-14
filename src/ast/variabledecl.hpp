#ifndef VARIABLEDECL_HPP
#define VARIABLEDECL_HPP

#include "compiler/compiler.hpp"
#include "node.hpp"
#include "types.hpp"


namespace Ast {
class VariableDecl : public Node {
protected:
    String m_name;
    ValueType m_type;
    ExpressionPtr m_value;

public:
    VariableDecl(String name, ValueType type, ExpressionPtr value);

    virtual String compile(Comp::Compiler &) override;

    GetProperty(name);
    GetProperty(type);
    GetProperty(value);


private:
    addClassName(Variable);
};
}  // namespace Ast

#endif  // VARIABLEDECL_HPP
