import std.stdio;
import grammar.scot;
import args;
import errors;
import logger;
import cli;

int main(string[] args) {
    Args conf;

    try
        // If failed to parse args use the global fallback logger
        conf = Args(args);
    catch (Exception err)
        return handleError(fallBackLogger, err);

    // Logger constructor is nothrow, so shoudl be fine to do this
    auto log = new Logger(conf.verbosity, conf.color);

    try
        return cli.run(log, conf); // Execute main application
    catch (Exception err)
        return handleError(log, err); // handle any remaining exceptions.

}
