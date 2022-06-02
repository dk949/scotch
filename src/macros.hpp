#ifndef MACROS_HPP
#define MACROS_HPP

#define PropertyGet(NAME)                                    \
    m_##NAME;                                                \
public:                                                      \
    [[nodiscard]] inline const auto &NAME() const noexcept { \
        return m_##NAME;                                     \
    }                                                        \
private:

#define PropertyGetSet(NAME)                                 \
    m_##NAME;                                                \
public:                                                      \
    [[nodiscard]] inline const auto &NAME() const noexcept { \
        return m_##NAME;                                     \
    }                                                        \
    [[nodiscard]] inline auto &NAME() noexcept {             \
        return m_##NAME;                                     \
    }                                                        \
private:


#define MERGE_(a, b) a##b
#define LABEL_(a, b) MERGE_(a, b)

#define TRY(X)                                                  \
    ({                                                          \
        auto LABEL_(x, __LINE__) = (X);                         \
        if (!LABEL_(x, __LINE__))                               \
            return tl::unexpected(LABEL_(x, __LINE__).error()); \
        std::move(*LABEL_(x, __LINE__));                        \
    })

#define TRY_VOID(X)                                             \
    ({                                                          \
        auto LABEL_(v, __LINE__) = (X);                         \
        if (!LABEL_(v, __LINE__))                               \
            return tl::unexpected(LABEL_(v, __LINE__).error()); \
    })


#define VARIANT_CASE(VARIANT, T, VAR, B)           \
    case scotch::variantIndex<TypeVariant, T>(): { \
        auto &VAR = std::get<T>(VARIANT);          \
        B break;                                   \
    }


#ifdef NDEBUG
#define DEV(VAR)
#define BREAK()
#define DBG(X) X
#else
#include <csignal>
#define DEV(VAR) [[maybe_unused]] VAR;
#define BREAK()  std::raise(SIGTRAP)
#define DBG(X)                                         \
    ({                                                 \
        auto LABEL_(d, __LINE__) = (X);                \
        fmt::print(stderr, "{}", LABEL_(d, __LINE__)); \
        LABEL_(d, __LINE__);                           \
    })
#endif


#endif  // MACROS_HPP
