#ifndef ARGS_HPP
#define ARGS_HPP
#include "macros.hpp"

#include <string_view>
#include <vector>


struct Args {
public:
    enum class Verbosity { None, Some, Most, All };
    using enum Verbosity;

private:
    Args(std::vector<std::string_view> positionals, std::string_view output, Verbosity verb, bool tok, bool ast, bool wat);

    std::vector<std::string_view> PropertyGet(positionals)
    std::string_view PropertyGet(output)
    Verbosity PropertyGet(verbosity)
    bool PropertyGet(dumpTokens)
    bool PropertyGet(dumpAst)
    bool PropertyGet(dumpWat)

public:

    static Args parse(char **argv);
};

// Printing
#include <fmt/format.h>
template<>
struct fmt::formatter<Args::Verbosity> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(Args::Verbosity verb, FormatContext &ctx) {
        using enum Args::Verbosity;
        std::string name = "Invalid Verbosiy level";
        switch (verb) {
            case None:
                name = "None";
                break;
            case Some:
                name = "Some";
                break;
            case Most:
                name = "Most";
                break;
            case All:
                name = "All";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

template<>
struct fmt::formatter<Args> : formatter<std::string> {
    template<typename FormatContext>
    auto format(Args args, FormatContext &ctx) {
        return formatter<std::string>::format(fmt::format("({{\n"
                                                          "\tpositionals: [{}]\n"
                                                          "\toutput: {}\n"
                                                          "\tverbosity: {}\n"
                                                          "\tdumpTokens: {}\n"
                                                          "\tdumpAst: {}\n"
                                                          "\tdumpWat: {}\n"
                                                          "}}",
                                                  fmt::join(args.positionals(), ", "),
                                                  args.output(),
                                                  args.verbosity(),
                                                  args.dumpTokens(),
                                                  args.dumpAst(),
                                                  args.dumpWat()),
            ctx);
    }
};

#endif  // ARGS_HPP
