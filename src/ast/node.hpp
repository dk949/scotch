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
String printNode(NodePtr node);

class Node {
public:

    virtual StringView className() const = 0;

    virtual ~Node() = default;

    virtual std::partial_ordering operator<=>(const Node &rhs) const {
        trace();
        return this <=> &rhs;
    }

    friend String Ast::printNode(NodePtr node);

protected:
    Node() = default;
};



}  // namespace Ast

template<>
struct fmt::formatter<Ast::NodePtr> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Ast::NodePtr n, FormatContext &ctx) {
        return formatter<std::string>::format(Ast::printNode(n), ctx);
    }
};

#endif  // NODE_HPP
