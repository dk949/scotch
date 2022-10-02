module frontend.utils.get;
@safe:

import grammar.scot;
import grammar.scot.rules;
import utils;

string getMatchAt(string func = __FUNCTION__)(ref const(ParseTree) cst, size_t idx, Rule rule) {
    assertF!(func)(cst[idx].name == rule, cst[idx].name ~ " != " ~ rule);
    return cst.matches[idx];
}

ref const(ParseTree) getChildAt(string func = __FUNCTION__)(
    return ref const(ParseTree) cst,
    size_t idx,
    Rule rule
) {
    assertF!(func)(cst[idx].name == rule, cst[idx].name ~ " != " ~ rule);
    return cst[idx];
}

@property
size_t length(ref const(ParseTree) cst) {
    return cst.children.length;
}
