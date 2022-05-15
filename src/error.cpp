#include "error.hpp"

Error::Error()
        : msg() { }
Error::Error(const char *m)
        : msg(m) { }
Error::Error(std::string_view m)
        : msg(m) { }
Error::Error(std::string m)
        : msg(std::move(m)) { }
