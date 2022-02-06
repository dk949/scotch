#ifndef VALUE_HPP
#define VALUE_HPP
#include "tools.hpp"
#include "types.hpp"

namespace Ast {

class Value {
public:
private:
    ValueType m_type;
    union {
        Int64 as_int;
        ValueType as_type;
    } m_value;

public:

    explicit Value(ValueType type)
            : m_type(type) { }

    explicit Value(Int64 val)
            : m_type(ValueType::INT) {
        m_value.as_int = val;
    }

    Int64 as_int() {
        verify(m_type == ValueType::INT);
        return m_value.as_int;
    }

    ValueType as_type() {
        verify(m_type == ValueType::TYPE);
        return m_value.as_type;
    }

    ValueType inline type() const {
        return m_type;
    }
};
}

template<>
struct fmt::formatter<Ast::Value> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Ast::Value t, FormatContext &ctx) {
        std::string name = "Invalid value";
        switch (t.type()) {  //
            bcase Ast::ValueType::INT : name  = fmt::format("Int({})", t.as_int());
            bcase Ast::ValueType::TYPE : name = fmt::format("Type({})", t.as_type());
        }

        return formatter<string_view>::format(name, ctx);
    }
};

#endif  // VALUE_HPP
