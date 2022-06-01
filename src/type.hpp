#ifndef TYPE_HPP
#define TYPE_HPP
#include "macros.hpp"

#include <limits>
#include <string_view>
#include <variant>
enum class Mod { NONE, CONST, LET };

enum class BuiltInType { i32, i64, f32, f64, _max };
struct UserType {
    size_t id;
    bool operator==(const UserType &) const = default;
};
struct UnknownType {
    bool operator==(const UnknownType &) const = default;
};

using TypeVariant = std::variant<BuiltInType, UserType, UnknownType>;

class Type {
private:
    TypeVariant PropertyGet(type)

    Type(TypeVariant &&);
public:
    Type() = default;
    static Type fromTypeVariant(TypeVariant);
    static Type fromBuiltinType(BuiltInType);
    static Type fromUserType(UserType);
    static Type fromUnknownType();

    bool operator==(const Type &) const = default;
    [[nodiscard]] size_t id() const;
    static constexpr size_t unknownId = std::numeric_limits<size_t>::max();
};

#include "printers.hpp"

#endif  // TYPE_HPP
