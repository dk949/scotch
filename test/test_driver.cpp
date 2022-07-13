#include "common.hpp"
#include "empty_post_proc.hpp"
#include "empty_pre_proc.hpp"
#include "parser_interface.hpp"
#include "parser_options.hpp"
#include "pipeline.hpp"
#include "test_io.hpp"
#include "wasm_compiler.hpp"

#include <catch.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <project/config.hpp>
#include <vector>
namespace fs = std::filesystem;


Pipeline makeTestPipeline(std::string &out, Error &err) {
    return Pipeline {//
        scotch::makeVector<std::unique_ptr<Preprocessor>>(std::make_unique<EmptyPreproc>()),
        std::make_unique<WasmCompiler>(),
        scotch::makeVector<std::unique_ptr<Postprocessor>>(std::make_unique<EmptyPostproc>()),
        Io::makeIo<TestOutput, TestError>(std::make_tuple(&out), std::make_tuple(&err))};
}


using TestFiles = std::vector<std::pair<fs::path, std::ifstream>>;

TestFiles openTestFiles(fs::path testdir) {
    const fs::directory_iterator dirContents {fs::path(scotch::project::dirs::test) / testdir};
    std::vector<std::pair<fs::path, std::ifstream>> vec;
    std::transform(fs::begin(dirContents), fs::end(dirContents), std::back_inserter(vec), [](const auto &dirent) {
        auto path = fs::path {dirent};
        auto file = std::ifstream {path};
        if (!file) {
            throw std::runtime_error {"file " + path.string() + " does not exist"};
        }
        return std::make_pair(std::move(path), std::move(file));
    });

    return vec;
}

void runTests(TestFiles &&testFiles) {
    for (auto &[path, i] : testFiles) {
        INFO("Test " << path << '\n');
        const auto pathStr = path.string();
        std::string out;
        Error err;

        ParserOptions::clearPipeline();
        ParserOptions::setPipeline(makeTestPipeline(out, err));
        ParserInterface parser {&i};
        int res = parser.parse();
        if (pathStr.ends_with("error")) {
            if (pathStr.ends_with("parse.error")) {
                CHECK(res != 0);
            } else if (pathStr.ends_with("compile.error")) {
                CHECK(!err.msg.empty());
            } else {
                INFO("Invalid test");
                CHECK(false);
            }
        } else {
            CHECK(res == 0);
            UNSCOPED_INFO("Compiler error message = " << err.msg);
            CHECK(err.msg.empty());
        }
    }
}


TEST_CASE("declarations") {
    runTests(openTestFiles("declaration_tests"));
}

TEST_CASE("functions") {
    runTests(openTestFiles("function_tests"));
}

TEST_CASE("parameters") {
    runTests(openTestFiles("parameter_tests"));
}

TEST_CASE("condition") {
    runTests(openTestFiles("if_tests"));
}
