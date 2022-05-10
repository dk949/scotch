link_compile_commands()
set(EXE_NAME scotch)

find_package(FLEX)
flex_target(lexer "${CMAKE_SOURCE_DIR}/src/scotch.l" ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

find_package(BISON)
bison_target(parser "${CMAKE_SOURCE_DIR}/src/scotch.y" ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_executable(
    ${EXE_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)
enable_sanitizers(${EXE_NAME})
set_target_warnings(${EXE_NAME})
target_include_directories(${EXE_NAME} PRIVATE "${CMAKE_BINARY_DIR}/cmake_config/include")
target_include_directories(${EXE_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${EXE_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")


find_package(fmt CONFIG REQUIRED)
target_link_libraries(${EXE_NAME} PRIVATE fmt::fmt)

