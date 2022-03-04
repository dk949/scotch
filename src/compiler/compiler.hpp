#ifndef COMPILER_HPP
#define COMPILER_HPP
#include "types.hpp"

#include <fmt/format.h>

namespace Comp {

class Compiler {
private:
    Ast::ProgramPtr m_prog;
public:
    explicit Compiler(Ast::ProgramPtr);

    String compile();
};

}



template<>
struct fmt::formatter<Comp::Compiler> : formatter<std::string> {
    template<typename FormatContext>
    auto format(const Comp::Compiler &, FormatContext &ctx) {
        std::string name = "Empty compiler";

        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // COMPILER_HPP
