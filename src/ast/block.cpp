#include "block.hpp"

#include "log.hpp"

namespace Ast {

String Block::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}
