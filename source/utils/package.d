module utils;

public import utils.stack;

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
T declval(T)();

/// Not implemented
void notImplemented(string msg = null) {
    assert(0, "Not implemented" ~ (msg ? " " ~ msg : ""));
}

/// unreachable
void unreachable(string msg = null) {
    assert(0, "Unreachable" ~ (msg ? " " ~ msg : ""));
}

template ident(T) {
    T delegate(T) ident = (T t) => t;
}
