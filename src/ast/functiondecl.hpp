#ifndef FUNCTIONDECL_HPP
#define FUNCTIONDECL_HPP

#include "node.hpp"   // for Node, addClassName
#include "types.hpp"  // for ValueType, String, ScopePtr, Vector, StringView


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
        trace();
        return m_name;
    }

private:
    addClassName(FunctionDecl);
};
}

#endif  // FUNCTIONDECL_HPP
