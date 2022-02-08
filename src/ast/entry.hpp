#ifndef ENTRY_HPP
#define ENTRY_HPP


#include "functiondecl.hpp"
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
