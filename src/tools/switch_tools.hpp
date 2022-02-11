#ifndef SWITCH_TOOLS_HPP
#define SWITCH_TOOLS_HPP
#include "hash.hpp"
#include "log.hpp"

#define bcase \
    break;    \
    case


#define str_switch(STR) switch (Tools::fnv_1a((STR)))
#define str_case(STR)   case Tools::fnv_1a((STR))



#define ENUM_DO(E)

#define ENUM_CASE(E) \
    case E:          \
        ENUM_DO(E);  \
        break
#define BAD_ENUM_CASE(E) \
    case E:              \
        unreachable("case {} should never happen", #E);

#endif // SWITCH_TOOLS_HPP

