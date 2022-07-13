import std.stdio;
import std.file;
import pegged.grammar;

void main() {
    asModule("scot", "grammar/scot", std.file.readText("grammargen/views/" ~ "scot.pegd"));
}
