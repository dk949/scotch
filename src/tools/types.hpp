#ifndef TYPES_HPP
#define TYPES_HPP
#include "ast_fwd.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// This is not very elegant, but it works better then all the other methods
#define MakePtr std::make_shared
// can't get this working atm
// alias_fn(MakePtr, std::make_shared);


// Containers
template<typename T>
using Ptr = std::shared_ptr<T>;
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using Optional = std::optional<T>;
template<typename U, typename T>
using Map = std::unordered_map<U, T>;

// Strings
using StringView = std::string_view;
using String = std::string;

// other
using Int64 = int64_t;
using Hash = int64_t;

namespace Ast {

// Pointers
using NodePtr = Ptr<Node>;
using ExpressionPtr = Ptr<Expression>;
using ScopePtr = Ptr<Scope>;
using ValuePtr = Ptr<Value>;
using ProgramPtr = Ptr<Program>;
using FunctionDeclPtr = Ptr<FunctionDecl>;
using BlockPtr = Ptr<Block>;

// Containers of things
using NodePtrVector = Vector<Ptr<Node>>;

enum class BinOp { PLUS, MINUS };

enum class ValueType { INT, TYPE };
}

#endif  // TYPES_HPP
