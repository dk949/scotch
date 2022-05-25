#include "io.hpp"

#include "error.hpp"


Io::Io(std::unique_ptr<Output> &&output, std::unique_ptr<ErrorHandler> &&err)
        : m_output(std::move(output))
        , m_error(std::move(err)) { }

void Io::output(const std::string &str) {
    m_output->output(str);
}

void Io::error(const Error &err) {
    m_error->error(err);
}
