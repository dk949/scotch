module utils.stack;

struct Stack(T) {
    import std.range;

    private T[] m_data;
    ref inout(T) top() inout {
        return m_data.back;
    }

    bool empty() {
        return m_data.empty;
    }

    size_t length() inout {
        return m_data.length;
    }

    void push(ref T elem) {
        m_data ~= elem;
    }

    void push(T elem) {
        m_data ~= elem;
    }

    T pop() {
        auto tmp = m_data.back;
        m_data.popBack;
        return tmp;
    }

    int opApply(scope int delegate(ref T) dg) {
        foreach_reverse (ref t; m_data) {
            if (const res = dg(t)) {
                return res;
            }
        }
        return 0;
    }
}

unittest {
    struct ComplexStruct {
        int a, b, c;
        float d, e, f;
        string g, h, i;
        ComplexStruct*[]* j, k, l;
    }

    alias S = Stack!ComplexStruct;

    auto s = S();
    assert(s.empty);
    s.push(ComplexStruct());
    ComplexStruct c;
    s.push(c);
    assert(!s.empty);
    assert(s.length == 2);
    assert(s.top is c);
    assert(s.pop() is c);
    assert(!s.empty);
    s.pop();
    assert(s.empty);
}

unittest {
    alias S = Stack!int;

    auto s = S();

    s.push(10);
    s.push(11);
    s.push(12);

    int[] arr;

    foreach (elem; s) {
        arr ~= elem;
    }
    assert(arr == [12, 11, 10]);
}
