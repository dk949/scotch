#include "common.hpp"

namespace scotch{
NotImplemented::NotImplemented(std::string_view what)
        : std::runtime_error(fmt::format("{} is not implemented", what)) { }

Unreachable::Unreachable(std::string_view what)
        : std::runtime_error(fmt::format("Reached an unreachable part of code in {}", what)) { }

NotSupported::NotSupported(std::string_view what)
        : std::runtime_error(fmt::format("{} is not (yet) supported", what)) { }
}
