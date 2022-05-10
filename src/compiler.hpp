#ifndef COMPILER_HPP
#define COMPILER_HPP


#include "ast_fwd.hpp"

#include <string>
#include <string_view>
#include <tl/expected.hpp>

struct Error {
    std::string msg;
};

template<typename T>
using ErrorOr = tl::expected<T, Error>;

class Compiler {
public:
    [[nodiscard]] virtual ErrorOr<void> typeCheck(const Program &) = 0;
    [[nodiscard]] virtual ErrorOr<std::string> compile(const Program &) = 0;
    [[nodiscard]] virtual std::string_view compilerType() const = 0;
    virtual ~Compiler() = default;
};

#endif  // COMPILER_HPP
