#include "block.hpp"

#include "log.hpp"

namespace Ast {

String Block::compile(Comp::Compiler &comp) {
    ftrace();
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}
