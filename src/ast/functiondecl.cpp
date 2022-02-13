#include "functiondecl.hpp"

#include <utility>  // for move


Ast::FunctionDecl::FunctionDecl(String name, Vector<ValueType> args, ValueType ret, ScopePtr body)
        : m_name(std::move(name))
        , m_args(std::move(args))
        , m_return(ret)
        , m_body(std::move(body)) {
    ftrace();
}
