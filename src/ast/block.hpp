#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "scope.hpp"

namespace Ast {
class Block : public Scope {
public:
    Block() = default;

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Block);
};
}  // namespace Ast

#endif  // BLOCK_HPP
