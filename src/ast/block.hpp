#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "scope.hpp"

namespace Ast {
class Block : public Scope {
public:
    Block() = default;
    friend String Ast::printNode(NodePtr node);

private:
    addClassName(Block);
};
}  // namespace Ast

#endif  // BLOCK_HPP
