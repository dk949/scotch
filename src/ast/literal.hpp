#ifndef LITERAL_HPP
#define LITERAL_HPP

#include "ast/node.hpp"
#include "ast/value.hpp"
#include "compiler/compiler.hpp"
#include "expression.hpp"
#include "types.hpp"

namespace Ast {
class Literal : public Expression {

private:
    Value m_value;
public:
    explicit Literal(Value value);

    inline Value &value() {
        return m_value;
    }

    inline const Value &value() const {
        return m_value;
    }

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Literal);
};

}  // namespace Ast
#endif  // LITERAL_HPP
