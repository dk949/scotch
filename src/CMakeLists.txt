# Generate compile_commands.json to make it easier to work with clang based tools
link_compile_commands()

set(EXE_NAME scotch)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_executable(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES})

message(STATUES ${CONAN_INCLUDE_DIRS_TL-EXPECTED})

target_include_directories(
    ${EXE_NAME}
    PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_SPDLOG}
    PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_FMT}
    PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_MS-GSL}
    PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_TL-EXPECTED}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/src/tools
)
target_link_libraries(
    ${EXE_NAME}
    PRIVATE project_options
            project_warnings
            CONAN_PKG::spdlog
            CONAN_PKG::ms-gsl
            CONAN_PKG::tl-expected
            ${BOOST_PKG}
)

if (ENABLE_TRACING)
    target_compile_definitions(${EXE_NAME} PRIVATE TRACE)
endif ()

target_compile_definitions(${EXE_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)
target_compile_definitions(${EXE_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:SCOTCH_CAN_HAVE_ALLOCA>)
target_compile_definitions(${EXE_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:SCOTCH_CAN_HAVE_ALLOCA>)
target_compile_definitions(${EXE_NAME} PRIVATE $<$<CXX_COMPILER_ID:Clang>:SCOTCH_CAN_HAVE_ALLOCA>)
