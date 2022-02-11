#include "warncahce.hpp"

std::unordered_set<uint64_t> WarnCache::m_cache {};
bool WarnCache::isCached(uint64_t hash, bool needsCache) {
    if (!needsCache)
        return false;

    if (!m_cache.contains(hash)) {
        m_cache.insert(hash);
        return false;
    }
    return true;
}
