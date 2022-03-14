#ifndef VALUETYPE_HPP
#define VALUETYPE_HPP
#include "switch_tools.hpp"
#include "ftrace.hpp"

#include <fmt/format.h>

namespace Ast {
enum class ValueType {
    I32,
    I64,

    F32,
    F64,

    TYPE,
};
}

template<>
struct fmt::formatter<Ast::ValueType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(Ast::ValueType v, FormatContext &ctx) {
        ftrace();
        std::string_view name = "Invalid value";
        switch (v) {  //
            bcase Ast::ValueType::I32 : {
                name = "i32";
            }
            bcase Ast::ValueType::I64 : {
                name = "i64";
            }
            bcase Ast::ValueType::F32 : {
                name = "f32";
            }
            bcase Ast::ValueType::F64 : {
                name = "f64";
            }
            bcase Ast::ValueType::TYPE : {
                name = "Type";
            }
        }

        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // VALUETYPE_HPP
