#include "log.hpp"
log_init();

#include "args.hpp"
#include "ast.hpp"
#include "common.hpp"
#include "file.hpp"
#include "parse.hpp"



dev const Lex::Builtins blt;
int main(int, dev char **argv) {
    ftrace();
    auto child = MakePtr<Ast::Return>(MakePtr<Ast::Literal>(Ast::Value {10}));

    auto mainContents = MakePtr<Ast::Block>();
    mainContents->append(child);

    auto scotchMain = MakePtr<Ast::FunctionDecl>("main", Vector<Ast::ValueType> {}, Ast::ValueType::INT, mainContents);

    auto program = MakePtr<Ast::Program>();
    program->append(scotchMain);

    info("target ast = \n{}", to<Ast::NodePtr>(program));

    Tools::Args::parse(argv);
    Vector<StringView> positionals = Tools::Args::positionals;
    if (positionals.empty()) {
        crash("{}", "Expected file, got nothing");
    }
    String input = Tools::loadFile(positionals.back());
    info("input = \n{}", input);

    Lex::Lexer l {input};
    const auto tokens = l.parseAll();
    for (const auto &tok : tokens) {
        info(tok);
    }

    Parser p {tokens};
    const auto ast = p.makeProgram();

    info("generated ast = \n{}", to<Ast::NodePtr>(ast));

    return 0;
}
