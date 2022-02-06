#include "tools.hpp"
log_init();

#include "args.hpp"
#include "ast.hpp"
#include "file.hpp"
#include "parse.hpp"
#include "tokenize.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


int main(int, dev char **argv) {
    // auto child = MakePtr<Ast::Return>(MakePtr<Ast::Literal>(Ast::Value {0}));

    // auto mainContents = MakePtr<Ast::Block>();
    // mainContents->append(child);

    // auto scotchMain = MakePtr<Ast::Entry>(mainContents);

    // auto program = MakePtr<Ast::Program>();
    // program->append(scotchMain);

    Args::parse(argv);
    Vector<StringView> positionals = Args::positionals;
    if (positionals.empty()) {
        crash("{}", "Expected file, got nothing");
    }
    String input = loadFile(positionals.back());
    info("input = \n{}", input);

    Lexer l {input};
    const auto tokens = l.parseAll();
    for (const auto &tok : tokens) {
        info(tok);
    }

    Parser p {tokens};
    dev const auto ast = p.makeNode();

    return 0;
}
