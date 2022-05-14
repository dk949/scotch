cmake_minimum_required(VERSION 3.18)
project(
    scotch
    VERSION 0.1.0
    DESCRIPTION "A statically typed programming language compiling to WebAssembly."
    HOMEPAGE_URL "https://github.com/dk949/scotch"
    LANGUAGES CXX
)


if(MSVC)
    message(FATAL_ERROR "MSVC is not supported due to the use of extensuions.\nSee the README for information on supported compilers and compiling on Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(cmake/cache.cmake)
include(cmake/iwyu.cmake)
include(cmake/sanitizers.cmake)
include(cmake/compile_commands.cmake)
include(cmake/compiler_warnings.cmake)
include(cmake/doxygen.cmake)

add_subdirectory(cmake_config/)
add_subdirectory(src)
