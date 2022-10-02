module utils;

@safe:
import std.traits;
import std.array;

public import utils.stack;
public import utils.set;

void append(T, Args...)(ref T a, auto ref Args args)
if (isInstanceOf!(Appender, T))
{
    foreach (arg; args) {
        a.put(arg);
    }
}

/**
 * Declares a private member variable `m_name` and a public memebr function
 * `name` returning it.
 *
 * If size of `T` is greater than size of size_t (i.e. it probably doesn't fit
 * into a register), a `ref const(T)` will be returned instead.
 *
 * Params:
 *   T = Type of property
 *   name = name of property
 */
mixin template propGet(alias T, string name) {

    mixin("private " ~ T.stringof ~ " m_" ~ name ~ ";");

    // dfmt off
    static if (T.sizeof <= size_t.sizeof)
        mixin("
            pure @safe @nogc nothrow
            const("~ T.stringof ~ ") " ~ name ~ "() const {
                return m_"
                ~ name ~ ";
            }
            ");
    else
        mixin("
            pure @safe @nogc nothrow
            ref const(" ~ T.stringof ~ ") " ~ name ~ "() const return {
                return m_"
                ~ name ~ ";
            }
            ");
    // dfmt on
}

/// Works like C++ std::declval.
ref T declval(T)();

/**
 * This is a valid path for the application, but it is not yet available
 *
 * Like assert(0), but with a nicer message and a clear intent
 */
noreturn notImplemented(string func = __FUNCTION__)(string msg = null) {
    assert(0, "n\n\n" ~ func ~ ": not implemented" ~ (msg ? ": " ~ msg : "") ~ "\n\n\n");
}

/**
 * This is an invalid path for the application.
 *
 * Like assert(0), but with a nicer message and a clear intent
 */
noreturn unreachable(string func = __FUNCTION__)(string msg = null) {
    assert(0, "\n\n\n" ~ func ~ ": unreachable" ~ (msg ? ": " ~ msg : "") ~ "\n\n\n");
}

/// Like notImplemented, but in debug mode will print a message instead of crashing the program
void placeholder(string func = __FUNCTION__)(string msg = null) {
    debug {
        import std.stdio;

        writeln("[PLACEHOLDER] ", func, (msg ? ": " ~ msg : ""));
    } else {
        notImplemented!(func)(msg);
    }

}

/// Like a regular assert, but makes the message more prominent
void assertF(string func = __FUNCTION__)(bool b, string msg = null) {
    assert(b, "n\n\n" ~ func ~ ": assertion failed" ~ (msg ? ": " ~ msg : "") ~ "\n\n\n");
}

T assertNotNull(T, string func = __FUNCTION__)(auto ref T val)
if (isNullAssignable!T) {
    assertF!func(val !is null);
    return val;
}

/// Creates a delegate which returns it's input. Useful for SumType
template ident(T) {
    T delegate(T) ident = (T t) => t;
}

enum isNullAssignable(T) = __traits(compiles, declval!T = null);

/// isNullAssignable
unittest {
    // dfmt off

    // True for:
    // Arrays: pinters, static, dynamic, associative (also string)
    static assert(isNullAssignable!(int*));
    static assert(isNullAssignable!(int[1]));
    static assert(isNullAssignable!(int[]));
    static assert(isNullAssignable!(int[string]));
    static assert(isNullAssignable!string);

    // Functions: function pointer, delegate
    static assert(isNullAssignable!(void function()));
    static assert(isNullAssignable!(void delegate()));

    // Classes
    class C {}
    static assert(isNullAssignable!C);

    // Null
    static assert(isNullAssignable!(typeof(null)));

    // False for:
    // Scalars
    static assert(!isNullAssignable!int);

    // Structs and unions
    struct S {}
    union U {}
    static assert(!isNullAssignable!S);
    static assert(!isNullAssignable!U);

    // SIMD vectors
    static assert(!isNullAssignable!(__vector(float[4])));

    // Void
    static assert(!isNullAssignable!void);
    // dfmt on
}
