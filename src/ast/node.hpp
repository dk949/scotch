#ifndef NODE_HPP
#define NODE_HPP

#include "value.hpp"
#include "types.hpp"

#include <compare>



#define addClassName(CLASS)                         \
    virtual StringView className() const override { \
        return #CLASS;                              \
    }

namespace Ast {

class Node {
public:

    virtual StringView className() const = 0;

    virtual ~Node() = default;

    virtual std::partial_ordering operator<=>(const Node &rhs) const {
        return this <=> &rhs;
    }

protected:
    Node() = default;
};



}  // namespace Ast

#endif  // NODE_HPP
