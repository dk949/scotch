#ifndef VARIABLEACCESS_HPP
#define VARIABLEACCESS_HPP

#include "expression.hpp"

namespace Ast {

class VariableAccess : public Expression {

private:
    String m_name;

public:
    explicit VariableAccess(String name);

    GetProperty(name);

    virtual String compile(Comp::Compiler &) override;


private:
    addClassName(VariableAccess);
};

}  // namespace Ast

#endif  // VARIABLEACCESS_HPP
