#ifndef CONSOLE_ERROR_HPP
#define CONSOLE_ERROR_HPP

#include "error_handler.hpp"

class ConsoleError : public ErrorHandler {
    void error(Error &&) override;
    [[nodiscard]] virtual constexpr std::string_view errorHandlerType() const override {
        return "ConsoleError";
    }
};
#endif  // CONSOLE_ERROR_HPP
