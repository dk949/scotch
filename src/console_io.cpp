#include "console_io.hpp"

#include "error.hpp"

#include <fmt/format.h>

void ConsoleOut::output(const std::string &str) {
    fmt::print("{}\n", str);
}

void ConsoleError::error(const Error &err) {
    fmt::print("An error occured:\n{}\n", err.msg);
}
