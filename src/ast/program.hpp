#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "compiler/compiler.hpp"
#include "scope.hpp"


namespace Ast {
class Program : public Scope {

public:
    explicit Program() = default;

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Program);
};
}  // namespace Ast

#endif  // PROGRAM_HPP
