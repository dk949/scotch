#ifndef FILE_HPP
#define FILE_HPP

#include "types.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <sstream>

namespace Tools {
String loadFile(StringView filename);
}

#endif  // FILE_HPP
