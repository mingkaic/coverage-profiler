This is project applys some dynamic analysis to extract the block coverage of a program.

Building with CMake
==============================================
1. Create a new directory for building.

        mkdir coverageprofilerbuild

2. Change into the new directory.

        cd coverageprofilerbuild

3. Run CMake with the coverage to the LLVM source.

        cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True \
            -DLLVM_DIR=</path/to/LLVM/build>/lib/cmake/llvm/ ../path-profiler-template

4. Run make inside the build directory:

        make

This produces a coverage profiler tool called `bin/coverageprofiler` and supporting
libraries in `lib/`.

Note, building with a tool like ninja can be done by adding `-G Ninja` to
the cmake invocation and running ninja instead of make.

Running
==============================================

First suppose that you have a program compiled to bitcode:

    clang -g -c -emit-llvm -S ../coverage-profiler/test/c/03-example.cpp -o example.ll

Running the coverage profiler:

    bin/coverageprofiler example.ll -o example
    ./example

