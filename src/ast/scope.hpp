#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "node.hpp"
namespace Ast {
class Scope : public Node {
private:
    NodePtrVector m_children;

public:
    const inline NodePtrVector &children() const {
        trace();
        return m_children;
    }

    template<typename T>
    void append(Ptr<T> child) {
        trace();
        m_children.emplace_back(child);
    }

    friend String Ast::printNode(NodePtr node);

protected:
    Scope() = default;
};
}  // namespace Ast

#endif  // SCOPE_HPP
