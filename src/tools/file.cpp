#include "file.hpp"

#include "ftrace.hpp"
#include "log.hpp"

String Tools::loadFile(StringView filename) {
    ftrace();
    std::ifstream is {filename.data()};  // :(
    if (!is)
        crash("File error: {}: {}", std::strerror(errno), filename);

    // jank, but will do for now
    std::stringstream buf;
    buf << is.rdbuf();
    return buf.str();
}
