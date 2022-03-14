#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "node.hpp"
namespace Ast {
class Scope : public Node {
protected:
    Vector<NodePtr> m_children;

public:
    GetProperty(children);

    template<typename T>
    void append(Ptr<T> child) {
        ftrace();
        m_children.emplace_back(child);
    }


protected:
    Scope() = default;
};
}  // namespace Ast

#endif  // SCOPE_HPP
