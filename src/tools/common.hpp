#ifndef COMMON_HPP
#define COMMON_HPP
#include "types.hpp"

namespace Tools {
template<typename>
constexpr bool always_false_v = false;
}

template<typename T>
constexpr T to(auto o) {
    return static_cast<T>(o);
}
template<typename T>
constexpr T is(auto *o) {
    return dynamic_cast<T>(o);
}
template<typename T, typename U>
constexpr T *is(Ptr<U> o) {
    return dynamic_cast<T *>(o.get());
}
template<typename T>
constexpr T as(auto o) {
    return reinterpret_cast<T>(o);
}

#define dev [[maybe_unused]]

#endif  // COMMON_HPP
