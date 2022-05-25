#ifndef CONSOLE_IO_HPP
#define CONSOLE_IO_HPP

#include "io.hpp"

class ConsoleOut : public Io::Output {
    void output(const std::string &) override;
    [[nodiscard]] virtual constexpr std::string_view outputType() const override {
        return "ConsoleOut";
    }
};

class ConsoleError : public Io::ErrorHandler {
    void error(const Error &) override;
    [[nodiscard]] virtual constexpr std::string_view errorHandlerType() const override {
        return "ConsoleError";
    }
};

#endif  // CONSOLE_IO_HPP
