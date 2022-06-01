#ifndef POST_PROCESSOR_HPP
#define POST_PROCESSOR_HPP
#include "error.hpp"

class Postprocessor {
public:
    [[nodiscard]] virtual ErrorOr<std::string> postprocess(std::string &&) const = 0;
    [[nodiscard]] virtual constexpr std::string_view postprocessorType() const = 0;

    virtual ~Postprocessor() = default;
};

#endif  // POST_PROCESSOR_HPP
