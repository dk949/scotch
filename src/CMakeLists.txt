link_compile_commands()

find_package(FLEX)
find_package(BISON)
find_package(fmt CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_path(INCLUDE_DIRS "fmt/format.h")

flex_target(lexer "${CMAKE_SOURCE_DIR}/src/scotch.l" ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

bison_target(
    parser "${CMAKE_SOURCE_DIR}/src/scotch.y" ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-Wcounterexamples"
)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp")


add_library(
    ${OBJLIB_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)

target_include_directories(${OBJLIB_NAME} PUBLIC "${CMAKE_BINARY_DIR}/cmake_config/include")
target_include_directories(${OBJLIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${OBJLIB_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(${OBJLIB_NAME} PUBLIC "${INCLUDE_DIRS}")

add_executable(${EXE_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp" $<TARGET_OBJECTS:${OBJLIB_NAME}>)

target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_BINARY_DIR}/cmake_config/include")
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(${EXE_NAME} PUBLIC "${INCLUDE_DIRS}")

target_link_libraries(${EXE_NAME} PUBLIC fmt::fmt)
target_link_libraries(${EXE_NAME} PUBLIC tl::expected)

enable_sanitizers(${EXE_NAME})
set_target_warnings(${EXE_NAME})
