#ifndef HASH_HPP
#define HASH_HPP

#include <string_view>
namespace tools {

constexpr uint64_t fnv_1a(std::string_view data) {
    uint64_t hash = 0xcbf29ce484222325;

    constexpr uint64_t prime = 0x00000100000001B3;

    for (const auto ch : data) {
        // uses static_cast instead of `to` to avoid circular dependency
        hash ^= static_cast<uint64_t>(ch);
        hash *= prime;
    }
    return hash;
}

}

#endif  // HASH_HPP
