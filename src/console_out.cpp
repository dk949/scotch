#include "console_out.hpp"

#include <fmt/format.h>

void ConsoleOut::output(std::string &&str) const {
    fmt::print("{}\n", str);
}

