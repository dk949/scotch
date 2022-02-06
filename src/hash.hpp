#ifndef HASH_HPP
#define HASH_HPP

#include <string_view>
namespace CTHash {

template<typename Size>
constexpr Size fnv_1a(std::string_view data) {
    Size hash = sizeof(Size) == 4 ? 0x811c9dc5 : sizeof(Size) == 8 ? 0xcbf29ce484222325 : 0;

    constexpr Size prime = sizeof(Size) == 4 ? 0x01000193 : sizeof(Size) == 8 ? 0x00000100000001B3 : 0;

    for (const auto ch : data) {
        hash ^= static_cast<Size>(ch);
        hash *= prime;
    }
    return hash;
}

}  // namespace CTHash

#endif  // HASH_HPP
