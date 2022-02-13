#ifndef FUNCTIONDECL_HPP
#define FUNCTIONDECL_HPP

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

    const inline String &name() const {
        return m_name;
    }

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(FunctionDecl);
};
}

#endif  // FUNCTIONDECL_HPP
