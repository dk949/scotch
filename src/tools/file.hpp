#ifndef FILE_HPP
#define FILE_HPP

#include "class_tools.hpp"
#include "scalloca.hpp"
#include "types.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <gsl/gsl>
#include <sstream>
#include <tl/expected.hpp>

namespace Tools {

#define svalloca(SV)     scalloca(SV.size() + 1)
#define DECL_FN(Ret, Fn) [[nodiscard]] FileErrorOr<Ret> Fn noexcept

class File;
struct FileError;

template<typename T>
using FileErrorOr = tl::expected<T, FileError>;

struct FileError {
    StringView errStr;
    int errc;
};


class File {
public:
    enum State : unsigned char {
        Read = 0b0001,
        Write = 0b0010,
        Append = 0b0100,
        Binary = 0b1000,
    };
private:
    struct StateRepr {
        char read   : 1;
        char write  : 1;
        char append : 1;
        char binary : 1;
        char padding: 4;
    };
    static_assert(sizeof(StateRepr) == sizeof(uint8_t));

    gsl::owner<FILE *> m_fp;

    static constexpr auto maxStackString = 512;

    File(FILE *) noexcept;

public:
    NO_COPY(File);

    File(File &&) noexcept;
    File &operator=(File &&) = delete;
    static FileErrorOr<File> open(StringView filename, uint8_t mode) noexcept;
    FileErrorOr<void> close() noexcept;


    ~File() noexcept;

    DECL_FN(String, read());
    DECL_FN(void, write(StringView data));
    DECL_FN(void, write(const String &data));
    DECL_FN(void, write(const char *data, size_t size));

    DECL_FN(const char *, static svToCharPtr(void *dest, StringView source));
    DECL_FN(const char *, static stateToMode(uint8_t state));
    DECL_FN(String, static readFile(StringView filename));
    DECL_FN(void, static writeFile(StringView location, StringView data));
};
}  // namespace Tools

#endif  // FILE_HPP
