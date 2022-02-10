#ifndef NODE_HPP
#define NODE_HPP

#include "types.hpp"
#include "value.hpp"

#include <compare>



#define addClassName(CLASS)                         \
    virtual StringView className() const override { \
        trace();                                    \
        return #CLASS;                              \
    }

namespace Ast {

class Node {
public:

    virtual StringView className() const = 0;

    virtual ~Node() = default;

    virtual std::partial_ordering operator<=>(const Node &rhs) const {
        trace();
        return this <=> &rhs;
    }

protected:
    Node() = default;
};



}  // namespace Ast

#endif  // NODE_HPP
