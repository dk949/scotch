## Build Dependencies

### Required

* GCC 11+
  * Any compiler which implements GCC extensions (such as clang) can also be
    used.
* [cmake](https://cmake.org/)
* flex and bison
  * May work with other implementations if lex and yacc, but only these were
    tested.

### Optional

* [bldr](https://github.com/dk949/bldr)
* [ccache](https://ccache.dev/)
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
* [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
* [cmake-format](https://github.com/cheshirekow/cmake_format)
* [cppcheck](http://cppcheck.sourceforge.net/)
* [dot](https://graphviz.org/)
* [doxygen](https://www.doxygen.nl/index.html)

## Runtime dependencies

The compiler generates WebAssembly text format (WAT). This has to be translated
into a binary. The binary needs to be loaded into a WebAssembly runtime and
executed.

This functionality is provided by `wasmrunner` and requires
[node](https://nodejs.org/) and [yarn](https://yarnpkg.com/).


## Building

By far the simplest option is to use bldr. Below are the required bldr commands
followed by the equivalent manual steps.

**Initialise debug project**

``` sh
bldr init
```

``` sh
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
./vcpkg/vcpkg install
yarn --cwd wasmrunner
cmake -B build/debug -S . -DCMAKE_BUILD_TYPE=Debug -DENABLE_CACHE=ON -DENABLE_DOXYGEN=OFF
```

**Build**

``` sh
bldr build
```

``` sh
cmake --build build/debug -j 12
```

**Test**

``` sh
bldr test
```

``` sh
./build/debug/bin/test
```
