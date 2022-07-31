import std.stdio;
import std.file;
import pegged.grammar;

void main() {
    write("Writing generated grammar........");
    asModule("scot", "grammar/scot", std.file.readText("grammargen/views/" ~ "scot.pegd"));
    writeln("complete.");
}
