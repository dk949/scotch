#ifndef ARGS_HPP
#define ARGS_HPP
#include "types.hpp"


namespace Tools {
struct Args {
private:
    Args(Vector<StringView> positionals, StringView output);
    Vector<StringView> m_positionals;
    StringView m_output;
public:
    static Args parse(char **argv);

    inline Vector<StringView> positionals() const noexcept {
        return m_positionals;
    }
    inline StringView output() const noexcept {
        return m_output;
    }
};
}


#endif  // ARGS_HPP
