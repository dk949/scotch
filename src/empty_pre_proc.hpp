#ifndef EMPTY_PRE_PROC_HPP
#define EMPTY_PRE_PROC_HPP

#include "ast.hpp"
#include "pre_processor.hpp"

class EmptyPreproc : public Preprocessor {
    [[nodiscard]] ErrorOr<Program> preprocess(Program &&prog) const override {
        return prog;
    }
    [[nodiscard]] constexpr std::string_view preprocessorType() const override {
        return "EmptyPreproc";
    }
};

#endif  // EMPTY_PRE_PROC_HPP
