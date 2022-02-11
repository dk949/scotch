#ifndef WARNCAHCE_HPP
#define WARNCAHCE_HPP

#include <cstdint>
#include <unordered_set>
#include <vector>
class WarnCache {
    static std::unordered_set<uint64_t> m_cache;

public:
    static bool isCached(uint64_t hash, bool needsCache = true);
};

#endif  // WARNCAHCE_HPP
