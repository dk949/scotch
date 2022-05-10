#ifndef MACROS_HPP
#define MACROS_HPP

#define PropertyGet(NAME)                                    \
    m_##NAME;                                                \
public:                                                      \
    [[nodiscard]] inline const auto &NAME() const noexcept { \
        return m_##NAME;                                     \
    }                                                        \
private:

#endif // MACROS_HPP

