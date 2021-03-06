#ifndef TYPE_STORE_HPP
#define TYPE_STORE_HPP
#include "error.hpp"
#include "type.hpp"

#include <fmt/format.h>
#include <fwd.hpp>
#include <string>
#include <unordered_map>


// FIXME: currently scopes are not handled at all
//   Solution: have a TypeStore for each scope. When looking up types,
//   look through each accessible TypeStore starying from the smallest scope.

struct TypeRepr {
    // TODO: this only allows for product types not sum types.
    std::vector<BuiltInType> repr;
    [[nodiscard]] ErrorOr<BuiltInType> asBuiltin() const;
};

class TypeStore {
private:
    using TypeMap = std::unordered_map<std::string, size_t>;
    using ReprMap = std::unordered_map<size_t, TypeRepr>;
    TypeMap m_typeIds;
    ReprMap m_reprs;
    size_t m_next;
public:
    TypeStore();
    Type makeType(const std::string &);
    bool contains(const Type &) const;
    bool contains(size_t) const;
    bool contains(const std::string &) const;
    Type operator[](const std::string &) const;
    const TypeRepr &operator[](const Type &) const;
    const TypeRepr &operator[](size_t) const;
    const std::string &string(const Type &) const;
    const std::string &string(size_t) const;
    friend struct fmt::formatter<TypeStore>;
};


// Printing
#include <numeric>
template<>
struct fmt::formatter<TypeStore> : formatter<std::string> {
    template<typename FormatContext>
    auto format(TypeStore store, FormatContext &ctx) {
        using namespace std::literals;
        auto maxLen = std::transform_reduce(  //
            store.m_typeIds.begin(),
            store.m_typeIds.end(),
            "Type Name"sv.size(),
            std::max<size_t>,
            [](auto &&elem) -> size_t { return elem.first.size(); });

        auto str = fmt::format("\n|{:<{}}|{:>8}|\n", "Type name", maxLen, "Type ID");
        for (const auto &[k, v] : store.m_typeIds) {
            fmt::format_to(std::back_inserter(str), "|{:<{}}|{:>8}|\n", k, maxLen, v);
        }
        return formatter<string_view>::format(str, ctx);
    }
};


#endif  // TYPE_STORE_HPP
