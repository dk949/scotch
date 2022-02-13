# Automatically enable catch2 to generate ctest targets
link_compile_commands()

set(EXE_NAME tests)

if (CONAN_CATCH2_ROOT_DEBUG)
    include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else ()
    include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif ()

file(
    GLOB
    TEST_FILES
    "${CMAKE_SOURCE_DIR}/test/*.cpp"
)
file(
    GLOB_RECURSE
    TESTED_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
)

list(FILTER TEST_FILES EXCLUDE REGEX "^.*/test/catch_main.cpp$")
list(FILTER TESTED_FILES EXCLUDE REGEX "^.*/src/main.cpp$")





if(ENABLE_TRACING)
    target_compile_definitions(${EXE_NAME} PRIVATE TRACE)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)



add_executable(${EXE_NAME}  ${TESTED_FILES} ${TEST_FILES})
target_link_libraries(${EXE_NAME} PRIVATE project_warnings project_options catch_main
        CONAN_PKG::spdlog
        CONAN_PKG::ms-gsl
        )

target_include_directories(
        ${EXE_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/src/tools
)

target_compile_definitions(${EXE_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)
catch_discover_tests(
    tests
    TEST_PREFIX
    "unittests."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "unittests."
    OUTPUT_SUFFIX
    .xml
)
