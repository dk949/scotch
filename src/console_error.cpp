#include "console_error.hpp"

#include <fmt/format.h>


void ConsoleError::error(Error &&err) const {
    fmt::print("An error occured:\n{}\n", err.msg);
}
