#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "ast/valuetype.hpp"
#include "types.hpp"

#include <fmt/format.h>
#include <ranges>

namespace Comp {

struct LocalRep {
    StringView name;
    String decl;

    constexpr bool operator==(StringView other) const {
        return name == other;
    }
    constexpr bool operator==(const LocalRep &other) const {
        return name == other.name;
    }
};

struct FuncRep {
    StringView name;
    Vector<Ast::ValueType> args;
    Ast::ValueType ret;
    Vector<LocalRep> locals;
};


class Compiler {
private:
    Ast::ProgramPtr m_prog;
    Vector<FuncRep> m_funcs;
    StringView m_moduleName;
public:
    explicit Compiler(Ast::ProgramPtr);

    String compile();
    void appendFunc(StringView, const Vector<Ast::ValueType> &, Ast::ValueType, const std::vector<LocalRep> &locals = {});
    void appendFunc(const FuncRep &);
    void appendVar(const LocalRep &var);
    void setModName(StringView);

    inline const Vector<FuncRep> &funcs() const {
        return m_funcs;
    }
};

}  // namespace Comp



template<>
struct fmt::formatter<Comp::FuncRep> : formatter<std::string> {
    template<typename FormatContext>
    auto format(const Comp::FuncRep &fun, FormatContext &ctx) {
        std::string name;
        fmt::format_to(std::back_inserter(name),
            "{}({}) -> {}, Locas[{}]",
            fun.name,
            fmt::join(fun.args, ","),
            fun.ret,
            fmt::join(fun.locals | std::views::transform([](const Comp::LocalRep &local) { return local.name; }), ","));
        return formatter<string_view>::format(name, ctx);
    }
};


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
