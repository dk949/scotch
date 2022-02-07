#include "token_type.hpp"

template<>
struct fmt::formatter<TokenType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(TokenType t, FormatContext &ctx) {
        std::string name = "Invalid token";
        FOREACH_TOKEN(t);
        return formatter<string_view>::format(name, ctx);
    }
};
