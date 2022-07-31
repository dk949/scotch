module errors;
import logger;
import utils;
import std.exception;

private enum : int {
    Unhandled = 4,
    Internal,
}

class ArgumentException : Exception {
    mixin basicExceptionCtors;
}

class CliException : Exception {
    mixin basicExceptionCtors;
}

class ExitException : Exception {
    mixin propGet!(int, "exitCode");

    @nogc @safe nothrow pure
    this(int exitCode,
        string msg = null,
        string file = __FILE__,
        size_t line = __LINE__,
        Throwable next = null) {
        super(msg, file, line);
        m_exitCode = exitCode;
    }
}

int handleExitException(const Logger log, ExitException e) {
    if (e.exitCode == 0)
        log.infoln(e.message);
    else
        log.error(e.message);
    return e.exitCode;
}

class InternalError : Error {
    @safe pure nothrow
    this(string msg,
        string file = __FILE__,
        size_t line = __LINE__,
        Throwable next = null) {
        super("Internal Error: " ~ msg, file, line, next);
    }
}

class FrontendException : Exception {
    mixin basicExceptionCtors;
}

int handleNullException(const Logger log) {
    log.error("Internal Error: received null exception");
    return Internal;
}

int handleUnhandled(const Logger log, Exception e) {
    log.error(typeid(e), ": ", e.message);
    return Unhandled;
}

int handleError(const Logger log, Exception err) {
    import std.algorithm.comparison;
    import std.functional;

    try
        return err.castSwitch!(
            partial!(handleExitException, log),
            partial!(handleNullException, log),
            partial!(handleUnhandled, log),
        );
    catch (Exception e) {
        log.error("Internal Error: Unhandled exception: ", typeid(e), e.message);
        return Internal;
    }
}
