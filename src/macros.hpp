#ifndef MACROS_HPP
#define MACROS_HPP

#define PropertyGet(NAME)                                    \
    m_##NAME;                                                \
public:                                                      \
    [[nodiscard]] inline const auto &NAME() const noexcept { \
        return m_##NAME;                                     \
    }                                                        \
private:


#define MERGE_(a, b) a##b
#define LABEL_(a)    MERGE_(x_, a)

#define TRY(X)                                               \
    ({                                                       \
        auto LABEL_(__LINE__) = (X);                         \
        if (!LABEL_(__LINE__))                               \
            return tl::unexpected(LABEL_(__LINE__).error()); \
        std::move(*LABEL_(__LINE__));                        \
    })

#define TRY_VOID(X)                                          \
    ({                                                       \
        auto LABEL_(__LINE__) = (X);                         \
        if (!LABEL_(__LINE__))                               \
            return tl::unexpected(LABEL_(__LINE__).error()); \
    })

#endif  // MACROS_HPP
