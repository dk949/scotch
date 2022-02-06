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
    SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/test"
    "*.cpp"
)
list(FILTER SOURCE_FILES EXCLUDE REGEX "^.*/test/catch_main.cpp$")

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(${EXE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXE_NAME} PRIVATE project_warnings project_options catch_main)

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
