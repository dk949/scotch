#include "ast.hpp"
#include "common.hpp"

#include <fmt/format.h>


constexpr auto indentSize = 2;

template<int O>
constexpr unsigned char offs = O *indentSize;

String Ast::printNode(const NodePtr &node) {
    static int indent = 0;


    if (auto *func = is<FunctionDecl>(node); func) {
        // clang-format off
        String out {fmt::format("{: >{}}{}({} -> {})("
                                    "\n{: >{}}args(",
                "", indent, node->className(), func->m_name, func->m_return,
                "", indent + offs<1>)};
        // clang-format on

        indent += offs<2>;
        if (!func->m_args.empty()) {
            for (dev const auto &arg : func->m_args) {
                fmt::format_to(std::back_inserter(out), "\n{: >{}}{}", "", indent, arg);
            }
            fmt::format_to(std::back_inserter(out), "\n{: >{}})", "", indent - offs<1>);
        } else {
            out.push_back(')');
        }

        // clang-format off
        fmt::format_to(std::back_inserter(out), "\n{: >{}}body("
                "\n{}"
                "\n{: >{}})"
                "\n{: >{}})",
                "", indent - offs<1>,
                to<NodePtr>(func->m_body),
                "", indent - offs<1>,
                "", indent - offs<2>
                );
        // clang-format on
        indent -= offs<2>;
        return out;
    }

    if (auto *scope = is<Scope>(node); scope) {
        String out {fmt::format("{: >{}}{}(", "", indent, node->className())};
        indent += offs<1>;
        if (!scope->m_children.empty()) {
            for (dev const auto &child : scope->m_children) {
                fmt::format_to(std::back_inserter(out), "\n{}", child);
            }
            indent -= offs<1>;
            fmt::format_to(std::back_inserter(out), "\n{: >{}})", "", indent);
        } else {
            out.push_back(')');
        }
        return out;
    }

    if (auto *ret = is<Return>(node); ret) {
        indent += offs<1>;

        // clang-format off
        String out {
            fmt::format("{: >{}}{}("
                            "\n{}"
                        "\n{: >{}})",

                        "", indent - offs<1>, node->className(),
                        to<NodePtr>(ret->m_argument),
                        "", indent - offs<1>)
        };
        // clang-format on
        indent -= offs<1>;
        return out;
    }

    if (auto *binexpr = is<BinExpr>(node); binexpr) {
        indent += offs<1>;
        // clang-format off
        String out{
        fmt::format("{: >{}}{}("
                        "\n{}"
                        "\n{: >{}}{}"
                        "\n{}"
                    "\n{: >{}})",

                   "" , indent - offs<1>, node->className(),
                   to<NodePtr>(binexpr->m_lhs),
                   "" , indent - offs<0>, binexpr->m_op,
                   to<NodePtr>(binexpr->m_rhs),
                   "" , indent - offs<1>)
        };
         //clang-format on
        indent -= offs<1>;
        return out;
    }

    if (auto *literal = is<Literal>(node); literal) {
        return fmt::format("{: >{}}{}({})", "", indent, node->className(), literal->m_value);
    }

    return fmt::format("{: >{}}{}", "", indent, node->className());
}
