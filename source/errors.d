module errors;
import logger;
import utils;

class ArgumentException : Exception {
    @nogc @safe nothrow
    this(string msg) {
        super(msg);
    }
}

class CliException : Exception {
    @nogc @safe nothrow
    this(string msg) {
        super(msg);
    }
}

class ExitException : Exception {
    mixin(propGet("int exitCode"));
    @nogc @safe nothrow
    this(int exitCode, string msg = null) {
        super(msg);
        m_exitCode = exitCode;
    }
}

class InternalError : Error {
    @nogc @safe nothrow
    this(string msg) {
        super(msg);
    }
}

class FrontendException : Exception {
    @nogc @safe nothrow
    this(string msg) {
        super(msg);
    }

}

int handleError(const Logger log, const Exception err) {
    if (const exit = cast(ExitException) err) {
        if (exit.exitCode == 0)
            log.infoln(exit.message);
        else
            log.error(exit.message);
        return exit.exitCode;
    }
    log.error(err.message);
    return 1;
}
