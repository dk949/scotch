#ifndef NODE_HPP
#define NODE_HPP

#include "compiler/compiler.hpp"
#include "ftrace.hpp"
#include "types.hpp"
#include "value.hpp"

#define addClassName(CLASS)                                       \
    [[nodiscard]] virtual StringView className() const override { \
        ftrace();                                                 \
        return #CLASS;                                            \
    }

namespace Ast {
String printNode(const NodePtr &node);

class Node {
public:

    [[nodiscard]] virtual StringView className() const = 0;

    virtual ~Node() = default;

    virtual String compile(Comp::Compiler &) = 0;

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
