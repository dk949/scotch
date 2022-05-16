#ifndef COMPILER_HPP
#define COMPILER_HPP


#include "ast_fwd.hpp"
#include "error.hpp"

#include <string_view>

class Compiler {
public:
    [[nodiscard]] virtual ErrorOr<void> typeCheck(const Program &) = 0;
    [[nodiscard]] virtual ErrorOr<std::string> compile(Program &&) const = 0;
    [[nodiscard]] virtual constexpr std::string_view compilerType() const = 0;
    virtual ~Compiler() = default;
};

#endif  // COMPILER_HPP
