#ifndef PRE_PROCESSOR_HPP
#define PRE_PROCESSOR_HPP

#include "fwd.hpp"
#include "error.hpp"


class Preprocessor {
public:
    [[nodiscard]] virtual ErrorOr<Program> preprocess(Program &&) const = 0;
    [[nodiscard]] virtual constexpr std::string_view preprocessorType() const = 0;

    virtual ~Preprocessor() = default;
};

#endif  // PRE_PROCESSOR_HPP
