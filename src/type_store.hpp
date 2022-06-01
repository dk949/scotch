#ifndef TYPE_STORE_HPP
#define TYPE_STORE_HPP
#include "type.hpp"

#include <fmt/format.h>
#include <string>
#include <unordered_map>
class TypeStore {
private:
    using TypeMap = std::unordered_map<std::string, size_t>;
    TypeMap m_typeIds;
    size_t m_next;
public:
    TypeStore();
    Type makeType(const std::string &);
    bool contains(const Type &) const;
    bool contains(size_t) const;
    bool contains(const std::string &) const;
    Type operator[](const std::string &) const;
    const std::string &operator[](const Type &) const;
    const std::string &operator[](size_t) const;
    friend struct fmt::formatter<TypeStore>;
};

#include "printers.hpp"

#endif  // TYPE_STORE_HPP
