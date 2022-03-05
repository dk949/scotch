#ifndef COMPILER_HPP
#define COMPILER_HPP
#include "types.hpp"

#include <fmt/format.h>

namespace Comp {

class Compiler {
private:
    Ast::ProgramPtr m_prog;
    Vector<StringView> m_funcs;
public:
    explicit Compiler(Ast::ProgramPtr);

    String compile();
    void appendFunc(StringView);
    inline const Vector<StringView> &funcs() const {
        return m_funcs;
    }
};

}  // namespace Comp



template<>
struct fmt::formatter<Comp::Compiler> : formatter<std::string> {
    template<typename FormatContext>
    auto format(const Comp::Compiler &comp, FormatContext &ctx) {
        std::string name = "Comp(";
        if (comp.funcs().empty()) {
            name.append("Empty");
        } else {
            fmt::format_to(std::back_inserter(name), "Funcs[{}]", fmt::join(comp.funcs(), ", "));
        }
        name.push_back(')');

        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // COMPILER_HPP
