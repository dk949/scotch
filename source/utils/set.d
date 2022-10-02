module utils.set;

@safe struct Set(T) {
    private alias Unit = void[0];
    Unit[T] _set;

    void insert(ref const(T) t) {
        _set[t] = Unit.init;
    }

    void insert(T t) {
        _set[t] = Unit.init;
    }

}
