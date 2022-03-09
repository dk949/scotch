#ifndef VALUE_HPP
#define VALUE_HPP
#include "compiler/compiler.hpp"
#include "switch_tools.hpp"
#include "types.hpp"

#include <fmt/format.h>

namespace Ast {

class Value {
public:
private:
    ValueType m_type;
    union {
        Int32 as_i32;
        Int64 as_i64;
        Float32 as_f32;
        Float64 as_f64;
        ValueType as_type;
    } m_value;

public:

    explicit Value(ValueType type);
    explicit Value(Int32 val);
    explicit Value(Int64 val);
    explicit Value(Float32 val);
    explicit Value(Float64 val);

    String compile(Comp::Compiler &);

    Int32 as_i32();
    Int64 as_i64();

    Float32 as_f32();
    Float64 as_f64();

    ValueType as_type();
    [[nodiscard]] ValueType inline type() const {
        return m_type;
    }
};
}  // namespace Ast

template<>
struct fmt::formatter<Ast::Value> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Ast::Value t, FormatContext &ctx) {
        std::string name = "Invalid value";
        switch (t.type()) {  //
            bcase Ast::ValueType::I32 : {
                name = fmt::format("i32({})", t.as_i32());
            }
            bcase Ast::ValueType::I64 : {
                name = fmt::format("i64({})", t.as_i64());
            }
            bcase Ast::ValueType::F32 : {
                name = fmt::format("f32({})", t.as_i32());
            }
            bcase Ast::ValueType::F64 : {
                name = fmt::format("f64({})", t.as_f64());
            }
            bcase Ast::ValueType::TYPE : {
                name = fmt::format("Type({})", t.as_type());
            }
        }

        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // VALUE_HPP
