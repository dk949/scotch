#include "test_io.hpp"

#include "error.hpp"


TestOutput::TestOutput(std::string *s)
        : out(s) { }

void TestOutput::output(const std::string &s) {
    *out = s;
}

TestError::TestError(Error *e)
        : err(e) { }

void TestError::error(const Error &e) {
    *err = e;
}
