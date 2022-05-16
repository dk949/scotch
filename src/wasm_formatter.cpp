#include "wasm_formatter.hpp"

#include "common.hpp"

#include <fmt/format.h>
#include <numeric>

[[nodiscard]] ErrorOr<std::string> WasmFormatter::postprocess(std::string &&in) const {
    auto splitStr = scotch::splitString(in, '\n');
    std::string out;
    out.reserve(in.size() + splitStr.size() * 8);

    std::accumulate(splitStr.begin(), splitStr.end(), 0, [&out](ssize_t indent, std::string_view line) {
        indent += (std::count(line.begin(), line.end(), '(') - std::count(line.begin(), line.end(), ')'));
        fmt::format_to(std::back_inserter(out), "{}\n{: >{}}", line, "", indent * 2);
        return indent;
    });
    return out;
};
[[nodiscard]] constexpr std::string_view WasmFormatter::postprocessorType() const {
    return "WasmFormatter";
};
