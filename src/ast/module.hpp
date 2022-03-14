#ifndef MODULE_HPP
#define MODULE_HPP

#include "compiler/compiler.hpp"
#include "scope.hpp"

namespace Ast {
class Module : public Scope {
protected:
    String m_name;


public:
    explicit Module(String name);

    inline const String &name() {
        return m_name;
    }

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Module);
};


}  // namespace Ast

#endif  // MODULE_HPP