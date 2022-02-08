#ifndef ENTRY_HPP
#define ENTRY_HPP

#include "ast/node.hpp"      // for addClassName
#include "functiondecl.hpp"  // for FunctionDecl
#include "types.hpp"         // for ScopePtr, StringView

namespace Ast {
class Entry : public FunctionDecl {
private:

public:
    Entry(ScopePtr body);

private:
    addClassName(Entry);
};
}

#endif  // ENTRY_HPP
