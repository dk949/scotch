#ifndef FILE_HPP
#define FILE_HPP

#include "class_tools.hpp"
#include "types.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <gsl/gsl>
#include <sstream>

namespace Tools {

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
    static constexpr auto maxStackString = 4096;
public:
    NO_MOVE_OR_COPY(File);

    explicit File(StringView filename, uint8_t state);
    ~File();

    String read();
    void write(StringView data);
    void write(const String &data);

    static char *svToCharPtr(void *dest, StringView source);
    [[nodiscard]] static const char *stateToMode(uint8_t state);
private:
    void writeImpl(const char *data, size_t size);
};


String loadFile(StringView filename);
void saveFile(StringView data, StringView location);
}

#endif  // FILE_HPP
