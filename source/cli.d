module cli;

import logger;
import args;
import frontend;
import backend;
import errors;
import pegged.peg;
import std.file;

Backend selectBackend(Args.Backend back) {
    final switch (back) {
        case Args.Backend.JS:
            import backend.js;
            return new Js();
        case Args.Backend.LLVM:
            throw new CliException("LLVM backend is not yet supported");
        case Args.Backend.WASM:
            throw new CliException("WASM backend is not yet supported");
    }
}

void dumpCst(Args.CstDumpType type, lazy ParseTree cst) {
    import std.stdio;

    switch (type) {
        case Args.CstDumpType.None:
            break;
        case Args.CstDumpType.Text:
            writeln(cst);
            goto default;
        case Args.CstDumpType.HTML:
            import pegged.tohtml;

            const cst_ = cst;
            toHTML!(Expand.ifNotMatch)(cst_, stdout);
            goto default;
        default:
            throw new ExitException(0);
    }
}

int run(Logger log, const Args args) {
    auto input = readText(args.inputFile);
    auto front = new Frontend(log, input);
    dumpCst(args.dumpCst, front.cst);
    auto ast = front.parse();
    log.infoln(ast);

    auto back = selectBackend(args.backend);

    return 0;
}
