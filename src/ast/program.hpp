#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "compiler/compiler.hpp"
#include "module.hpp"


namespace Ast {
class Program : public Node {

private:
    Vector<ModulePtr> m_modules;

public:
    [[nodiscard]] const inline Vector<ModulePtr> &modules() const {
        return m_modules;
    }

    void append(ModulePtr child) {
        ftrace();
        m_modules.emplace_back(child);
    }

    explicit Program() = default;

    virtual String compile(Comp::Compiler &) override;

    friend String Ast::printNode(const NodePtr &node);

private:
    addClassName(Program);
};
}  // namespace Ast

#endif  // PROGRAM_HPP
