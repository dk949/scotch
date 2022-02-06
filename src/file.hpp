#ifndef FILE_HPP
#define FILE_HPP

#include "tools.hpp"
#include "types.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <sstream>

String loadFile(StringView filename) {
    std::ifstream is {filename.data()};  // :(
    if (!is)
        crash("File error: {}: {}", std::strerror(errno), filename);

    // jank, but will do for now
    std::stringstream buf;
    buf << is.rdbuf();
    return buf.str();
}

#endif  // FILE_HPP
