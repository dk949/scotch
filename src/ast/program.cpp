#include "program.hpp"


namespace Ast {

String Program::compile(Comp::Compiler &comp) {
    spdlog::debug("Current compiler state = {}", comp);
    todo();
}

}
