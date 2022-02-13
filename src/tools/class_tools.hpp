#ifndef CLASS_TOOLS_HPP
#define CLASS_TOOLS_HPP

#define DEF_MOVE(CLASS)        \
    CLASS(CLASS &&) = default; \
    CLASS &operator=(CLASS &&) = default

#define DEF_COPY(CLASS)             \
    CLASS(const CLASS &) = default; \
    CLASS &operator=(const CLASS &) = default

#define NO_MOVE(CLASS)        \
    CLASS(CLASS &&) = delete; \
    CLASS &operator=(CLASS &&) = delete

#define NO_COPY(CLASS)             \
    CLASS(const CLASS &) = delete; \
    CLASS &operator=(const CLASS &) = delete

#define MOVE_BUT_NO_COPY(CLASS) \
    NO_COPY(CLASS);             \
    DEF_MOVE(CLASS)

#define COPY_BUT_NO_MOVE(CLASS) \
    DEF_COPY(CLASS);            \
    NO_MOVE(CLASS)

#define NO_MOVE_OR_COPY(CLASS) \
    NO_MOVE(CLASS);            \
    NO_COPY(CLASS)

#define DEF_MOVE_AND_COPY(CLASS) \
    DEF_MOVE(CLASS);             \
    DEF_COPY(CLASS)

#endif  // CLASS_TOOLS_HPP
