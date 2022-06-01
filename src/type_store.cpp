#include "type_store.hpp"

#include "common.hpp"

#include <algorithm>

#define ADD_BUILTIN(T) std::string {#T}, static_cast<size_t>(BuiltInType::T)

TypeStore::TypeStore()
        : m_typeIds(TypeMap {//
              {ADD_BUILTIN(i32)},
              {ADD_BUILTIN(i64)},
              {ADD_BUILTIN(f32)},
              {ADD_BUILTIN(f64)}})
        , m_next(static_cast<size_t>(BuiltInType::_max)) { }

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

const std::string &TypeStore::operator[](const Type &t) const {
    return operator[](t.id());
}
const std::string &TypeStore::operator[](size_t s) const {
    for (const auto &[k, v] : m_typeIds)
        if (v == s)
            return k;
    throw scotch::Unreachable(fmt::format("in function {}: s = {}\nts = {}", __func__, s, *this));
}
