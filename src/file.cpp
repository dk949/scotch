#include "file.hpp"

String loadFile(StringView filename) {
    trace();
    std::ifstream is {filename.data()};  // :(
    if (!is)
        crash("File error: {}: {}", std::strerror(errno), filename);

    // jank, but will do for now
    std::stringstream buf;
    buf << is.rdbuf();
    return buf.str();
}
