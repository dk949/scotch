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
    ValueType m_return;
    ScopePtr m_body;


public:
    FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body);

    virtual String compile(Comp::Compiler &) override;

    [[nodiscard]] const inline String &name() const {
        return m_name;
    }

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(FunctionDecl);
};
}  // namespace Ast

#endif  // FUNCTIONDECL_HPP
