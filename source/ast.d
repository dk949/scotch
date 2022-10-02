module ast;
@safe:
import utils;
import std.array;
import std.conv;

private enum TAB = "    ";

enum Mod {
    CONST,
    LET,
}

struct Scope {
    Scope* outer;
    Type[NamespacedID] types;
    Var[string] variables;
}

struct Type {
}

struct Var {
    Mod mod;
    NamespacedID type;
    Scope* scope_;
}

abstract class Expr {
    Type type;
    protected abstract string exprToString(string indent = TAB ~ TAB) const;
    override string toString() const {
        return exprToString();
    }
}

class CompoundExpr : Expr {
    Expr[] statements;

    protected override string exprToString(string indent) const {

        auto a = appender!string;

        a.append("\n", indent, "(CompoundExpr ");
        foreach (expr; statements) {
            a.append(expr.exprToString(indent ~ TAB), " ");
        }
        a.append(")");
        return a.data;
    }
}

abstract class Cond : Expr {
    Expr test;
    this(Expr test) {
        this.test = test;
    }
}

class If : Cond {
    Expr ifTrue;
    Expr ifFalse;

    this(Expr test) {
        super(test);
    }

    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class Case : Cond {
    CaseVariant[] cases;
    this(Expr test, CaseVariant[] cases) {
        super(test);
        this.cases = cases;
    }

    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class CaseVariant : Expr {
    Var* var;
    Expr caseValue;
    Expr expr;
    this(Var* var, Expr val, Expr expr) {
        this.var = var;
        this.caseValue = val;
    }

    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class VarDecl : Expr {
    Var* var;
    Expr val;
    this(Var* var, Expr val) {
        this.var = var;
        this.val = val;
    }

    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class Return : Expr {
    Expr val;
    this(Expr val) {
        this.val = val;
    }

    protected override string exprToString(string indent) const {
        return "(Return " ~ val.exprToString(indent ~ TAB) ~ ")";
    }
}

class Fail : Expr {
    Expr val;
    this(Expr val) {
        this.val = val;
    }

    protected override string exprToString(string indent) const {
        return "(Fail " ~ val.exprToString(indent ~ TAB) ~ ")";
    }
}

class Break : Expr {
    Expr val;
    this(Expr val) {
        this.val = val;
    }

    protected override string exprToString(string indent) const {
        return "(Break " ~ val.exprToString(indent ~ TAB) ~ ")";
    }
}

class Continue : Expr {
    protected override string exprToString(string indent) const {
        return "(Continue)";
    }
}

mixin template litConstructor(T) {
    this(T val) {
        this.val = val;
    }
}

class BoolLiteral : Expr {
    bool val;
    mixin litConstructor!bool;
    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class StringLiteral : Expr {
    string val;
    mixin litConstructor!string;
    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class CharLiteral : Expr {
    char val;
    mixin litConstructor!char;
    protected override string exprToString(string indent) const {
        notImplemented;
    }
}

class IntLiteral : Expr {
    long val;
    mixin litConstructor!long;
    protected override string exprToString(string indent) const {
        return "(Int " ~ val.to!string ~ ")";
    }
}

class FloatLiteral : Expr {
    double val;
    mixin litConstructor!float;
    mixin litConstructor!double;
    protected override string exprToString(string indent) const {
        return "(Float " ~ val.to!string ~ ")";
    }
}

class Unsupported : Expr {
    protected override string exprToString(string indent) const {
        return "(Unsupported)";
    }

}

class EmptyExpr : Expr {
    protected override string exprToString(string indent) const {
        return "(EmptyExpr)";
    }
}

struct NamespacedID {
    string[] id;
    alias id this;
    this(typeof(null)) {
        id = null;
    }

    string toString() const {
        auto a = appender!string;
        if (id) {
            a.put(id[0]);
            if (id.length > 1)
                foreach (name; id)
                    a.append("::", name);
        }
        return a.data;
    }
}

NamespacedID unknownType = null;

struct Function {

    string name = null;
    NamespacedID[string] args = null;
    NamespacedID ret = null;
    NamespacedID err = null;
    Expr val;

    string toString() const {
        auto a = appender!string;
        a.append("(Function ", `"`, name, `"`, " (Agrs ");
        foreach (arg; args) {
            a.append(arg.to!string, " ");
        }
        a.append(") ",
            "(Return ", ret.to!string, ")",
            "(Error ", err.to!string, ")",
            "(Body ", val.to!string, ")",
            ")",
        );

        return a.data;
    }
}

struct Program {
    string modName;
    Function[] funcs;
    string toString() const {
        auto a = appender!string;
        a.append("(Module ", `"`, modName, `"`);
        foreach (func; funcs) {
            a.append("\n", TAB, func.to!string);
        }
        a.put("\n)");
        return a.data;
    }
}
