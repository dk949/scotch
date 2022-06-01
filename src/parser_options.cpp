#include "parser_options.hpp"

#include <fmt/format.h>

void ParserOptions::setPipeline(Pipeline &&a) {
    if (m_pipeline) {
        fmt::print(stderr, "Internal Error: Pipeline cannot be changed after it was set\n");
        exit(1);
    }
    m_pipeline.emplace(std::move(a));
}

std::optional<Pipeline> ParserOptions::m_pipeline{};

Pipeline ParserOptions::takePipeline() {
    if (!m_pipeline) {
        fmt::print(stderr, "Internal Error: Pipeline cannot be acceesd if not set\n");
        exit(1);
    }
    std::optional<Pipeline> ret {};
    m_pipeline.swap(ret);
    return std::move(ret.value());
}

void ParserOptions::clearPipeline() {
    m_pipeline.reset();
}
