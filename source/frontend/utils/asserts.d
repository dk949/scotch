module frontend.utils.asserts;
@safe:
import grammar.scot;
import grammar.scot.rules;
import utils;

void assertNumChild(string func = __FUNCTION__)(ref const(ParseTree) cst, size_t num) {
    import std.conv;

    assertF!(func)(cst.children.length == num,
        "expected expression to have "
            ~ num.to!string
            ~ " child"
            ~ (num > 1 ? "ren." : "."));
}

void assertNumMatches(string func = __FUNCTION__)(ref const(ParseTree) cst, size_t num) {
    import std.conv;

    assertF!(func)(cst.matches.length == num,
        "expected expression to have "
            ~ num.to!string
            ~ " match"
            ~ (num > 1 ? "es." : "."));
}

void assertSuccess(string func = __FUNCTION__)(ref const(ParseTree) cst) {
    assertF!(func)(cst.successful);
}

void assertType(string func = __FUNCTION__)(ref const(ParseTree) cst, Rule rule) {
    import std.conv;

    assertF!(func)(cst.name == rule, "exepected " ~ rule ~ ", but got " ~ cst.name);
}

noreturn cannotParse(string func = __FUNCTION__)(string name) {
    unreachable!(func)("cannot parse " ~ name);
}

noreturn unexpectedNumChildren(string func = __FUNCTION__)(size_t num) {
    import std.conv;

    unreachable!(func)("unexpected number of children: " ~ num.to!string);
}
