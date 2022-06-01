#ifndef PRINTERS_HPP
#define PRINTERS_HPP

#include "ast.hpp"
#include "common.hpp"
#include "type.hpp"
#include "type_store.hpp"

#include <fmt/format.h>
#include <numeric>

template<>
struct fmt::formatter<Ident> : formatter<std::string> {
    auto format(const Ident &id, format_context &ctx) {
        return formatter<std::string>::format(id.name(), ctx);
    }
};

template<>
struct fmt::formatter<Mod> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(Mod mod, FormatContext &ctx) {
        std::string name = "Invalid token";
        switch (mod) {
            case Mod::NONE:
                name = "NONE";
                break;
            case Mod::CONST:
                name = "CONST";
                break;
            case Mod::LET:
                name = "LET";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

template<>
struct fmt::formatter<BuiltInType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(BuiltInType mod, FormatContext &ctx) {
        std::string name = "Invalid type";
        switch (mod) {
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


#endif  // PRINTERS_HPP
