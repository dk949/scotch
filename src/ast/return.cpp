#include "return.hpp"

#include <utility>  // for move

Ast::Return::Return(ExpressionPtr argument)
        : m_argument(std::move(argument)) { }
