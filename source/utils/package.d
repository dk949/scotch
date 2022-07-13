module utils;

@safe
string propGet(string mem) {
    import std.array, std.format: format;

    const s = mem.split(' ');
    return format!"
        private %s m_%s;
        @nogc @safe nothrow
        %s %s() const{
            return m_%s;
        }"(s[0], s[1], s[0], s[1], s[1]);
}

string usingEnum(E)(string vis = "public") {
    import std.traits: EnumMembers;
    import std.conv: to;

    string o;
    static foreach (mem; EnumMembers!E) {
        o ~= (vis ~ " alias " ~ mem.to!string ~ " = " ~ E.stringof ~ "." ~ mem.to!string ~ ";");
    }
    return o;
}

