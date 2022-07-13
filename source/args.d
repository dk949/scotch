module args;
import std.getopt;
import logger;
import utils;
import errors;

struct Args {
    enum Backend {
        JS,
        WASM,
        LLVM
    }

    enum CstDumpType {
        None,
        Text,
        HTML
    }

    mixin(propGet("Backend backend"));
    mixin(propGet("string inputFile"));
    mixin(propGet("string outputFile"));
    mixin(propGet("bool color"));
    mixin(propGet("Verbosity verbosity"));
    mixin(propGet("bool dumpTokens"));
    mixin(propGet("CstDumpType dumpCst"));
    mixin(propGet("bool dumpAst"));

    this(string[] args) {
        bool ver;
        bool nocol;
        auto helpInformation = getopt(
            args,
            "output|o", "path to the otput file", &m_outputFile,
            "backend|b", "which backend to use", &m_backend,
            "verbose", "how verbose should the output be", &m_verbosity,
            "no-color", "how verbose should the output be", &nocol,
            "dump-tokens", "print token list", &m_dumpTokens,
            "dump-CST", "print concrete styntax tree", &m_dumpCst,
            "dump-AST", "print abstract styntax tree", &m_dumpAst,
            "version|v", "show version information and exit", &ver,
        );
        m_color = !nocol;

        if (helpInformation.helpWanted) {
            defaultGetoptPrinter("Usage: scotch [OPTIONS] FILE",
                helpInformation.options);
        }
        if (ver) {
            // TODO: Unhardcode version
            throw new ExitException(0, "v0.1.0");
        }

        if (args.length != 2)
            throw new ArgumentException("Expected input file. Use scotch -h for help");

        m_inputFile = args[1];
    }

}
