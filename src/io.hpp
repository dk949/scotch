#ifndef IO_HPP
#define IO_HPP



#include <concepts>
#include <memory>
#include <string>

struct Error;

struct Io {

public:
    class Output {
    public:
        virtual void output(const std::string &) = 0;
        [[nodiscard]] virtual constexpr std::string_view outputType() const = 0;
        virtual ~Output() = default;
    };

    class ErrorHandler {
    public:
        virtual void error(const Error &) = 0;
        [[nodiscard]] virtual constexpr std::string_view errorHandlerType() const = 0;
        virtual ~ErrorHandler() = default;
    };
private:
    std::unique_ptr<Output> m_output;
    std::unique_ptr<ErrorHandler> m_error;
public:
    Io(std::unique_ptr<Output> &&, std::unique_ptr<ErrorHandler> &&);

    void output(const std::string &);
    [[nodiscard]] constexpr std::string_view outputType() const {
        return m_output->outputType();
    }

    void error(const Error &);
    [[nodiscard]] constexpr std::string_view errorHandlerType() const {
        return m_output->outputType();
    }

    template<std::derived_from<Output> O, std::derived_from<ErrorHandler> E>
    [[nodiscard]] static inline Io makeIo() {
        return Io {std::make_unique<O>(), std::make_unique<E>()};
    }
    template<std::derived_from<Output> O, std::derived_from<ErrorHandler> E, typename... OCtor, typename... ECtor>
    [[nodiscard]] static inline Io makeIo(std::tuple<OCtor...> &&octor, std::tuple<ECtor...> &&ector) {
        return Io {std::apply(
                       []<typename... Ts>(Ts && ...args) { return std::make_unique<O>(std::forward<Ts>(args)...); },
                       std::forward<std::tuple<OCtor...>>(octor)),
            std::apply(
                []<typename... Ts>(Ts && ...args) { return std::make_unique<E>(std::forward<Ts>(args)...); },
                std::forward<std::tuple<ECtor...>>(ector))};
    }
};

#endif  // IO_HPP
