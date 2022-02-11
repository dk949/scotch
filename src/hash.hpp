#ifndef HASH_HPP
#define HASH_HPP

#include "conv.hpp"

#include <string_view>
namespace CTHash {

constexpr uint64_t fnv_1a(std::string_view data) {
    uint64_t hash = 0xcbf29ce484222325;

    constexpr uint64_t prime = 0x00000100000001B3;

    for (const auto ch : data) {
        hash ^= to<uint64_t>(ch);
        hash *= prime;
    }
    return hash;
}

}

#endif  // HASH_HPP
