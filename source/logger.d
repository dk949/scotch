module logger;
import utils;

enum LogType {
    Error,
    Important,
    Info,
    Verbose,
    Debug,
}

enum Verbosity {
    None, // Only in case of error
    Some, // Important information (like warnings?)
    All, // Debug info
}

enum Color {
    Black = "\x1b[0;30m",
    Red = "\x1b[0;31m",
    Green = "\x1b[0;32m",
    Yellow = "\x1b[0;33m",
    Blue = "\x1b[0;34m",
    Magenta = "\x1b[0;35m",
    Cyan = "\x1b[0;36m",
    White = "\x1b[0;37m",

    BlackBold = "\x1b[1;30m",
    RedBold = "\x1b[1;31m",
    GreenBold = "\x1b[1;32m",
    YellowBold = "\x1b[1;33m",
    BlueBold = "\x1b[1;34m",
    MagentaBold = "\x1b[1;35m",
    CyanBold = "\x1b[1;36m",
    WhiteBold = "\x1b[1;37m",

    BlackBright = "\x1b[0;90m",
    RedBright = "\x1b[0;91m",
    GreenBright = "\x1b[0;92m",
    YellowBright = "\x1b[0;93m",
    BlueBright = "\x1b[0;94m",
    MagentaBright = "\x1b[0;95m",
    CyanBright = "\x1b[0;96m",
    WhiteBright = "\x1b[0;97m",

    BlackBrightBold = "\x1b[0;90m",
    RedBrightBold = "\x1b[0;91m",
    GreenBrightBold = "\x1b[0;92m",
    YellowBrightBold = "\x1b[0;93m",
    BlueBrightBold = "\x1b[0;94m",
    MagentaBrightBold = "\x1b[0;95m",
    CyanBrightBold = "\x1b[0;96m",
    WhiteBrightBold = "\x1b[0;97m",

    Reset = "\x1b[0m",
}

class Logger {
    import std.stdio;

    @nogc @safe nothrow
    private string textColor(LogType t) const {
        if (!m_color)
            return "";
        final switch (t) {
            case LogType.Error:
                return Color.RedBold;
            case LogType.Important:
                return Color.YellowBold;
            case LogType.Info:
            case LogType.Verbose:
                return "";
            case LogType.Debug:
                return Color.BlackBright;
        }
    }

    @nogc @safe nothrow
    private string resetColor() const {
        return m_color ? Color.Reset : "";
    }

    mixin(propGet("Verbosity level"));
    mixin(propGet("bool color"));
    // TODO: file logging

    @trusted
    void dbg(Args...)(auto ref Args args) const {
        if (m_level == Verbosity.All)
            stdout.writeln(textColor(LogType.Debug), "[DEBUG]: ", args, resetColor);
    }

    @trusted
    void verbose(Args...)(auto ref Args args) const {
        if (m_level == Verbosity.Some)
            stdout.writeln(textColor(LogType.Debug), "[VERBOSE]: ", args, resetColor);
    }

    @trusted
    void info(Args...)(auto ref Args args) const
    if (!is(typeof(args[0]) == Color)) {
        stdout.write(args);
    }

    @trusted
    void info(Args...)(Color color, auto ref Args args) const {
        if (m_color)
            stdout.write(cast(string) color, args, resetColor);
        else
            stdout.write(args);
    }

    @trusted
    void infoln(Args...)(auto ref Args args) const
    if (!is(typeof(args[0]) == Color)) {
        stdout.writeln(args);
    }

    @trusted
    void infoln(Args...)(Color color, auto ref Args args) const {
        if (m_color)
            stdout.writeln(cast(string) color, args, resetColor);
        else
            stdout.writeln(args);
    }

    @trusted
    void important(Args...)(auto ref Args args) const {
        stdout.writeln(textColor(LogType.Important), "[IMPORTANT]: ", args, resetColor);
    }

    @trusted
    void error(Args...)(auto ref Args args) const {
        stderr.writeln(textColor(LogType.Error), "[ERROR]: ", args, resetColor);
    }

    @nogc @safe nothrow
    this(Verbosity level, bool color) {
        debug {
            m_level = Verbosity.All;
        } else {
            m_level = level;
        }
        m_color = color;
    }
}

static this() {
    fallBackLogger = new Logger(Verbosity.None, false);
}

Logger fallBackLogger;
