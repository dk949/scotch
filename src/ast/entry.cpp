#include "entry.hpp"

Ast::Entry::Entry(ScopePtr body)
        : FunctionDecl("main", {}, ValueType::INT, body) { }
