#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "scope.hpp"
namespace Ast {
class Program : public Scope {

public:
    explicit Program() = default;
    friend String Ast::printNode(NodePtr node);

private:
    addClassName(Program);
};
}  // namespace Ast

#endif  // PROGRAM_HPP
