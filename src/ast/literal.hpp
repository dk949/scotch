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

    GetProperty(value);

    virtual String compile(Comp::Compiler &) override;


private:
    addClassName(Literal);
};

}  // namespace Ast
#endif  // LITERAL_HPP
