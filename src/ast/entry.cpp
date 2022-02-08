#include "entry.hpp"

#include "ast/functiondecl.hpp"  // for FunctionDecl


Ast::Entry::Entry(ScopePtr body)
        : FunctionDecl("main", {}, ValueType::INT, body) { }
