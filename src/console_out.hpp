#ifndef CONSOLE_OUT_HPP
#define CONSOLE_OUT_HPP
#include "output.hpp"


class ConsoleOut : public Output {
    void output(std::string &&) const override;
    [[nodiscard]] virtual constexpr std::string_view outputType() const override {
        return "ConsoleOut";
    }
};

#endif  // CONSOLE_OUT_HPP
