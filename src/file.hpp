#ifndef FILE_HPP
#define FILE_HPP

#include "tools.hpp"
#include "types.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <sstream>

String loadFile(StringView filename);

#endif  // FILE_HPP
