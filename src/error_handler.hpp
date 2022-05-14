#ifndef ERORR_HANDLER_HPP
#define ERORR_HANDLER_HPP


#include "error.hpp"
class ErrorHandler {
public:
    virtual void error(Error &&) const = 0;
    [[nodiscard]] virtual constexpr std::string_view errorHandlerType() const = 0;
    virtual ~ErrorHandler() = default;
};
#endif  // ERORR_HANDLER_HPP
