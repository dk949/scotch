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
using Int32 = int32_t;
using Int64 = int64_t;
static_assert(sizeof(float) == 4, "size of float is asumed to be 32 bits");
static_assert(sizeof(double) == 8, "size of double is asumed to be 64 bits");
using Float32 = float;
using Float64 = double;
using Hash = int64_t;

namespace Ast {

// Pointers
using BlockPtr = Ptr<Block>;
using ExpressionPtr = Ptr<Expression>;
using FunctionDeclPtr = Ptr<FunctionDecl>;
using LiteralPtr = Ptr<Literal>;
using NodePtr = Ptr<Node>;
using ProgramPtr = Ptr<Program>;
using ReturnPtr = Ptr<Return>;
using ScopePtr = Ptr<Scope>;
using ValuePtr = Ptr<Value>;

// Containers of things
using NodePtrVector = Vector<Ptr<Node>>;

enum class BinOp { PLUS, MINUS };

enum class ValueType {
    I32,
    I64,

    F32,
    F64,

    TYPE,
};
}  // namespace Ast

#endif  // TYPES_HPP
