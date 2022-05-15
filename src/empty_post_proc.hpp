#ifndef EMPTY_POST_PROC_HPP
#define EMPTY_POST_PROC_HPP

#include "post_processor.hpp"

#include <iostream>

class EmptyPostproc : public Postprocessor {
    [[nodiscard]] virtual ErrorOr<std::string> postprocess(std::string &&bin) const override {
        return bin;
    };
    [[nodiscard]] virtual constexpr std::string_view postprocessorType() const override {
        return "EmptyPostproc";
    };
};

#endif  // EMPTY_POST_PROC_HPP
