#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>

class Output {
public:
    virtual void output(const std::string &) = 0;
    [[nodiscard]] virtual constexpr std::string_view outputType() const = 0;
    virtual ~Output() = default;
};

#endif  // OUTPUT_HPP
