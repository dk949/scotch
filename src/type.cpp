#include "type.hpp"

#include "common.hpp"

#include <fmt/format.h>


Type::Type(TypeVariant &&t)
        : m_type(std::move(t)) { }

Type Type::fromTypeVariant(TypeVariant b) {
    return Type {std::move(b)};
}
Type Type::fromBuiltinType(BuiltInType b) {
    return fromTypeVariant(TypeVariant {b});
}
Type Type::fromUserType(UserType b) {
    return fromTypeVariant(TypeVariant {b});
}
Type Type::fromUnknownType() {
    return fromTypeVariant(TypeVariant {UnknownType {}});
}
size_t Type::id() const {
    switch (m_type.index()) {
        VARIANT_CASE(m_type, BuiltInType, arg, {
            return static_cast<size_t>(arg);
        })
        VARIANT_CASE(m_type, UserType, arg, {
            return arg.id;
        })
        VARIANT_CASE(m_type, UnknownType, arg, {
            return unknownId;
        })
        default:
            throw 42;
    }
}
