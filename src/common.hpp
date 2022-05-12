#ifndef COMMON_HPP
#define COMMON_HPP

#include <fmt/format.h>
#include <stdexcept>
#include <string_view>

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



}  // namespace scotch
#endif  // COMMON_HPP
