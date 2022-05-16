#ifndef WASM_FORMATTER_HPP
#define WASM_POST_PROC_HPP

#include "post_processor.hpp"

#include <iostream>

class WasmFormatter : public Postprocessor {
    [[nodiscard]] virtual ErrorOr<std::string> postprocess(std::string &&bin) const override;
    [[nodiscard]] virtual constexpr std::string_view postprocessorType() const override;
};

#endif  // WASM_FORMATTER_HPP
