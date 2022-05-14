#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include <tl/expected.hpp>

struct Error {
    std::string msg;
};

template<typename T>
using ErrorOr = tl::expected<T, Error>;


#endif  // ERROR_HPP
