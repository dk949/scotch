#include "return.hpp"

Ast::Return::Return(ExpressionPtr argument)
        : m_argument(std::move(argument)) { }
