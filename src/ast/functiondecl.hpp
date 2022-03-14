#ifndef FUNCTIONDECL_HPP
#define FUNCTIONDECL_HPP

#include "compiler/compiler.hpp"
#include "node.hpp"
#include "types.hpp"


namespace Ast {
class FunctionDecl : public Node {
protected:
    String m_name;
    Vector<ValueType> m_args;
    ValueType m_ret;
    ScopePtr m_body;


public:
    FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body);

    virtual String compile(Comp::Compiler &) override;

    GetProperty(name);
    GetProperty(args);
    GetProperty(ret);
    GetProperty(body);


private:
    addClassName(FunctionDecl);
};
}  // namespace Ast

#endif  // FUNCTIONDECL_HPP
