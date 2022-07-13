module ast;

struct Type{}

class Expr{
    Type type;
}

struct Function {
    string name;
}

struct Program {
    string modName;
    Function[] funcs;
}
