import pegged.grammar;
import std.algorithm;
import std.array;
import std.file;
import std.range;
import std.regex;
import std.stdio;
import std.datetime.stopwatch;

void main() {
    mkdirRecurse("grammar/scot");
    auto sw = StopWatch(AutoStart.no);

    write("Generating grammar........");
    sw.start;
    asModule("scot", "grammar/scot/package", std.file.readText("grammargen/views/scot.pegd"));
    sw.stop;
    writeln("complete in ", sw.peek.total!"msecs", "ms");
    sw.reset;

    write("Generating rules enum........");
    sw.start;
    auto rules = readText("grammar/scot/package.d")
        .matchAll(ctRegex!(`"(Scotch)\.([^"]+)"`))
        .map!`a[2]`
        .array
        .sort
        .uniq;
    auto a = appender!string;
    a.put(
        "/+ DO NOT EDIT BY HAND!
This module was automatically generated from the file grammar/scot/package.d
+/
module scot.rules;

enum Rule {
");
    foreach (rule; rules) {
        a.put("    ");
        a.put(rule);
        a.put(` = "`);
        a.put("Scotch.");
        a.put(rule);
        a.put("\",\n");
    }
    a.put("}");
    a.data.toFile("grammar/scot/rules.d");
    sw.stop;
    writeln("complete in ", sw.peek.total!"msecs", "ms");
}
