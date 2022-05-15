#ifndef PRINTERS_HPP
#define PRINTERS_HPP

#include "ast.hpp"

#include <fmt/format.h>

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
            case Mod::CONST:
                name = "CONST";
                break;
            case Mod::LET:
                name = "CONST";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};


#endif  // PRINTERS_HPP
