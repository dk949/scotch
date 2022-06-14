#include "type_store.hpp"

#include "common.hpp"

#include <algorithm>


ErrorOr<BuiltInType> TypeRepr::asBuiltin() const {
    if (repr.size() > 1)
        return tl::unexpected {Error {"Type is not representable as a single builtin"}};
    return repr.back();
}


#define ADD_BUILTIN(T, ...)                                                     \
    do {                                                                        \
        m_typeIds[#T] = static_cast<size_t>(T);                                 \
        m_reprs[static_cast<size_t>(T)] = TypeRepr {std::vector {__VA_ARGS__}}; \
    } while (0)

TypeStore::TypeStore() {
    using enum BuiltInType;

    ADD_BUILTIN(boolean, i32);
    ADD_BUILTIN(i32, i32);
    ADD_BUILTIN(i64, i64);
    ADD_BUILTIN(f32, f32);
    ADD_BUILTIN(f64, f64);

    m_next = static_cast<size_t>(BuiltInType::_max);
}

Type TypeStore::makeType(const std::string &t) {
    if (auto it = m_typeIds.find(t); it != m_typeIds.end()) {
        if (it->second < static_cast<size_t>(BuiltInType::_max)) {
            return Type::fromBuiltinType(static_cast<BuiltInType>(it->second));
        } else {
            return Type::fromUserType(UserType {it->second});
        }
    } else {
        m_typeIds.insert({t, m_next++});
        return Type::fromUserType(UserType {m_next - 1});
    }
}
bool TypeStore::contains(const Type &t) const {
    return std::find_if(m_typeIds.begin(), m_typeIds.end(), [&t](auto &&elem) { return elem.second == t.id(); }) !=
           m_typeIds.end();
}
bool TypeStore::contains(size_t s) const {
    return std::find_if(m_typeIds.begin(), m_typeIds.end(), [&s](auto &&elem) { return elem.second == s; }) != m_typeIds.end();
}
bool TypeStore::contains(const std::string &str) const {
    return m_typeIds.contains(str);
}
Type TypeStore::operator[](const std::string &str) const {
    auto id = m_typeIds.at(str);

    if (id == Type::unknownId)
        return Type::fromUnknownType();
    else if (id < static_cast<size_t>(BuiltInType::_max))
        return Type::fromBuiltinType(static_cast<BuiltInType>(id));
    else
        return Type::fromUserType(UserType {id});
}

const TypeRepr &TypeStore::operator[](const Type &t) const {
    return operator[](t.id());
}
const TypeRepr &TypeStore::operator[](size_t s) const {
    return m_reprs.at(s);
}

const std::string &TypeStore::string(const Type &t) const {
    return string(t.id());
}
const std::string &TypeStore::string(size_t s) const {
    for (const auto &[k, v] : m_typeIds)
        if (v == s)
            return k;
    throw scotch::Unreachable(__func__);
}
