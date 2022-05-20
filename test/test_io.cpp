#include "test_io.hpp"


TestOutput::TestOutput(std::string *s)
        : out(s) { }

void TestOutput::output(std::string &&s) {
    *out = std::move(s);
}

TestError::TestError(Error *e)
        : err(e) { }

void TestError::error(Error &&e) {
    *err = std::move(e);
}
