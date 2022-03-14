#ifndef VARIABLEACCESS_HPP
#define VARIABLEACCESS_HPP

#include "expression.hpp"

namespace Ast {

class VariableAccess : public Expression {

private:
    String m_name;

public:
    explicit VariableAccess(String name);

    inline const String &name() const {
        return m_name;
    }

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(VariableAccess);
};

}  // namespace Ast

#endif  // VARIABLEACCESS_HPP
