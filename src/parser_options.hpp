#ifndef PARSER_OPTIONS_HPP
#define PARSER_OPTIONS_HPP


#include "pipeline.hpp"

#include <memory>
#include <optional>

struct ParserOptions {
private:

    static std::optional<Pipeline> m_pipeline;


public:

    static void setPipeline(Pipeline &&);
    static Pipeline takePipeline();
    static void clearPipeline();
};

#endif  // PARSER_OPTIONS_HPP
