#ifndef NODE_HPP
#define NODE_HPP

#include "ftrace.hpp"
#include "types.hpp"
#include "value.hpp"

#include <compare>



#define addClassName(CLASS)                                       \
    [[nodiscard]] virtual StringView className() const override { \
        ftrace();                                                 \
        return #CLASS;                                            \
    }

namespace Ast {
String printNode(const NodePtr &node);

class Node {
public:

    virtual StringView className() const = 0;

    virtual ~Node() = default;

    virtual std::partial_ordering operator<=>(const Node &rhs) const {
        ftrace();
        return this <=> &rhs;
    }

    friend String Ast::printNode(const NodePtr &node);

protected:
    Node() = default;
};



}  // namespace Ast

template<>
struct fmt::formatter<Ast::NodePtr> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Ast::NodePtr n, FormatContext &ctx) {
        return formatter<std::string>::format(Ast::printNode(std::move(n)), ctx);
    }
};

#endif  // NODE_HPP
