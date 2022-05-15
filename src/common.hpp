#ifndef COMMON_HPP
#define COMMON_HPP

#include <fmt/format.h>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace scotch {

struct NotImplemented : std::runtime_error {
    NotImplemented(std::string_view what = "");
};

struct Unreachable : std::runtime_error {
    Unreachable(std::string_view what = "");
};

struct NotSupported : std::runtime_error {
    NotSupported(std::string_view what = "");
};

constexpr std::uint64_t hash(std::string_view sv) {
    std::uint64_t hash = 0xcbf29ce484222325u;
    constexpr std::uint64_t prime = 0x00000100000001B3ul;
    for (const char ch : sv) {
        hash ^= static_cast<std::uint64_t>(ch);
        hash *= prime;
    }
    return hash;
}


template<template<class...> class Tt>
struct isTemplate {
    template<typename... T>
    struct Member : std::false_type { };

    template<typename... T>
    struct Member<Tt<T...>> : std::true_type { };
};

template<template<class...> class T, typename... V>
concept Template = isTemplate<T>::template Member<V...>::value;


template<typename T, typename... Ts>
std::vector<T> makeVector(T &&first, Ts &&...rest) {
    T tmp[sizeof...(Ts) + 1] {std::forward<T>(first), std::forward<Ts>(rest)...};
    return std::vector<T> {std::make_move_iterator(std::begin(tmp)), std::make_move_iterator(std::end(tmp))};
}

template<typename D>
concept StringSplitDelim = std::same_as<const char *, D> || std::same_as<std::string_view, D> || std::same_as<char, D>;

namespace detail {
    constexpr size_t delimLength(StringSplitDelim auto d) {
        return d.size();
    }

    template<>
    constexpr size_t delimLength<char>(char) {
        return 1;
    }

    template<>
    constexpr size_t delimLength<const char *>(const char *d) {
        return std::char_traits<char>::length(d);
    }
}

[[nodiscard]] std::vector<std::string_view> splitString(std::string_view s, StringSplitDelim auto delim) {
    std::vector<std::string_view> out;
    size_t start = 0U;
    auto end = s.find(delim);
    const auto dLength = detail::delimLength(delim);
    while (end != std::string::npos) {
        out.push_back(s.substr(start, end - start));
        start = end + dLength;
        end = s.find(delim, start);
    }
    out.push_back(s.substr(start, end - start));
    return out;
}


}  // namespace scotch
#endif  // COMMON_HPP
