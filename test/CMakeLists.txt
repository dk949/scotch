link_compile_commands()
find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)


file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/test/*.hpp")

add_executable(${TEST_EXE} $<TARGET_OBJECTS:${OBJLIB_NAME}> ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${TEST_EXE} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${TEST_EXE} PUBLIC fmt::fmt)
target_link_libraries(${TEST_EXE} PUBLIC tl::expected)
target_include_directories(${TEST_EXE} PUBLIC "${CMAKE_BINARY_DIR}/cmake_config/include")
target_include_directories(${TEST_EXE} PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(${TEST_EXE} PUBLIC "${CMAKE_BINARY_DIR}/src")
enable_sanitizers(${TEST_EXE})
set_target_warnings(${TEST_EXE})
