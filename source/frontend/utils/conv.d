module frontend.utils.conv;
@safe:

import grammar.scot;
import grammar.scot.rules;
import ast;
import frontend.utils.asserts;
import frontend.utils;
import utils;

NamespacedID toNamespacedID(ref const(ParseTree) cst) {
    cst.assertSuccess;
    cst.assertType(Rule.NamespacedID);
    NamespacedID output;
    // TODO: use appender
    for (size_t i; i < cst.children.length; i++)
        output.id ~= cst.getMatchAt(i, Rule.BareID);

    return output;
}

Mod toMod(ref const(ParseTree) cst) {
    cst.assertSuccess;
    cst.assertType(Rule.Mod);
    cst.assertNumChild(1);
    switch (cst[0].name) {
        case Rule.CONST:
            return Mod.CONST;
        case Rule.LET:
            return Mod.LET;
        default:
            unreachable("invalid mod " ~ cst[0].name);
    }

}
