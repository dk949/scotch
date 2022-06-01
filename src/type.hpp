#ifndef TYPE_HPP
#define TYPE_HPP
#include "macros.hpp"

#include <limits>
#include <string_view>
#include <variant>

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

// Printing
#include "common.hpp"

#include <fmt/format.h>

template<>
struct fmt::formatter<BuiltInType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(BuiltInType type, FormatContext &ctx) {
        std::string name = "Invalid type";
        switch (type) {
            case BuiltInType::i32:
                name = "i32";
                break;
            case BuiltInType::i64:
                name = "i64";
                break;
            case BuiltInType::f32:
                name = "f32";
                break;
            case BuiltInType::f64:
                name = "f64";
                break;
            case BuiltInType::_max:
                throw scotch::Unreachable(__func__);
        }
        return formatter<string_view>::format(name, ctx);
    }
};

template<>
struct fmt::formatter<UserType> : formatter<size_t> {
    template<typename FormatContext>
    auto format(UserType type, FormatContext &ctx) {
        return formatter<size_t>::format(type.id, ctx);
    }
};

template<>
struct fmt::formatter<UnknownType> : formatter<string_view> {
    template<typename FormatContext>
    auto format(UnknownType type, FormatContext &ctx) {
        return formatter<string_view>::format("?", ctx);
    }
};

template<>
struct fmt::formatter<Type> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Type type, FormatContext &ctx) {
        auto &var = type.type();
        switch (var.index()) {
            VARIANT_CASE(var, BuiltInType, a, {
                return formatter<std::string>::format(fmt::format("BuiltInType({})", a), ctx);
            })
            VARIANT_CASE(var, UserType, a, {
                return formatter<std::string>::format(fmt::format("UserType({})", a), ctx);
            })
            VARIANT_CASE(var, UnknownType, a, {
                return formatter<std::string>::format(fmt::format("UnknownType({})", a), ctx);
            })
            default:
                throw scotch::Unreachable(__func__);
        }
    }
};

#endif  // TYPE_HPP
