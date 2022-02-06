#ifndef FUNCTIONDECL_HPP
#define FUNCTIONDECL_HPP

#include "node.hpp"
namespace Ast {
class FunctionDecl : public Node {
protected:
    String m_name;
    Vector<ValueType> m_args;
    ValueType m_return;
    ScopePtr m_body;


public:
    FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body)
            : m_name(std::move(name))
            , m_args(std::move(args))
            , m_return(std::move(ret))
            , m_body(std::move(body)) { }

    const inline String &name() const {
        return m_name;
    }

private:
    addClassName(FunctionDecl);
};
}

#endif  // FUNCTIONDECL_HPP
