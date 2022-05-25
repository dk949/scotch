#include "console_error.hpp"

#include <fmt/format.h>


void ConsoleError::error(const Error &err) {
    fmt::print("An error occured:\n{}\n", err.msg);
}
