#ifndef MACROS_HPP
#define MACROS_HPP

#define PropertyGet(NAME)                                    \
    m_##NAME;                                                \
public:                                                      \
    [[nodiscard]] inline const auto &NAME() const noexcept { \
        return m_##NAME;                                     \
    }                                                        \
private:



#define TRY(X)                                \
    ({                                        \
        auto x = (X);                         \
        if (!x)                               \
            return tl::unexpected(x.error()); \
        std::move(*x);                        \
    })

#endif  // MACROS_HPP
