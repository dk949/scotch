#include "variabledecl.hpp"

#include "literal.hpp"
#include "variableaccess.hpp"

namespace Ast {

VariableDecl::VariableDecl(String name, ValueType type, ExpressionPtr value)
        : m_name {std::move(name)}
        , m_type {type}
        , m_value {std::move(value)} { }

String VariableDecl::compile(Comp::Compiler &comp) {
    fixme("Variable declartion shoudl save the type of the variable. so it can be looked up later as it cannot be "
          "deduced form variable access");

    comp.appendVar({m_name, fmt::format("(local ${} {})", m_name, m_type)});

    return fmt::format("(local.set ${} {})", m_name, m_value->compile(comp));
}

}
