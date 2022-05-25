#ifndef TEST_IO_HPP
#define TEST_IO_HPP

#include "io.hpp"
class TestOutput : public Io::Output {
    std::string *out;
    void output(const std::string &) override;
    [[nodiscard]] constexpr std::string_view outputType() const override {
        return "TestOutput";
    }
public:
    TestOutput(std::string *s);
};

class TestError : public Io::ErrorHandler {
    Error *err;
    void error(const Error &) override;
    [[nodiscard]] constexpr std::string_view errorHandlerType() const override {
        return "TestError";
    }
public:
    TestError(Error *e);
};

#endif  // TEST_IO_HPP
