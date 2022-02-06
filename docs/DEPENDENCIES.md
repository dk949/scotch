# Project dependencies

## Required

* A c++ compiler
  * Targeting c++20
  * Tested on gcc 11.1.0, should work on any other modern compiler
* [python](https://www.python.org/)
  * to install and run conan
* [conan](https://conan.io/)
  * package management
* [cmake](https://cmake.org/)
  * Cross platform build system generation

## Optional

* [doxygen](https://www.doxygen.nl/index.html)
  * Documentation generation
* [dot](https://graphviz.org/)
  * Used by doxygen to produce graphs
* [ccache](https://ccache.dev/)
  * Compiler caching (msvc not supported)
* [cppcheck](http://cppcheck.sourceforge.net/)
  * Linter
* [include-what-you-use](https://include-what-you-use.org)
  * Linter
* [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
  * Linter
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
  * Code formatter
* [cmake-format](https://github.com/cheshirekow/cmake_format)
  * cmake code formatter
