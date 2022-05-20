#ifndef TEST_IO_HPP
#define TEST_IO_HPP

#include "error_handler.hpp"
#include "output.hpp"

class TestOutput : public Output {
    std::string *out;
    void output(std::string &&) override;
    [[nodiscard]] constexpr std::string_view outputType() const override {
        return "TestOutput";
    }
public:
    TestOutput(std::string *s);
};
class TestError : public ErrorHandler {
    Error *err;
    void error(Error &&) override;
    [[nodiscard]] constexpr std::string_view errorHandlerType() const override {
        return "TestError";
    }
public:
    TestError(Error *e);
};

#endif  // TEST_IO_HPP
