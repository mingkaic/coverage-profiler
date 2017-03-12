# This makefile can help you to build test inputs for your callgraph profiler
# and produce easier to interpret images from its output. If you wish to
# build call graphs using your tool as well, you must define the PROFILER
# variable below to contain the path to your compiled callgraph-profiler
# binary. If you wish to compile your own tests, set the paths for CLANG and
# OPT, as well.
#
# To build LLVM assembly files from C++ source files:
#   make llvmasm
#
# To remove previous output & intermediate files:
#   make clean
#

LLVM_BIN     := /Users/cmk/llvm/bin
PROFILER     := ../cmake-build-default/bin/coverageprofiler
CLANG        := $(LLVM_BIN)/clang++
OPT          := $(LLVM_BIN)/opt
DOT          := dot
RM           := rm
SOURCE_FILES := $(sort $(wildcard cc/*.cpp))
ASM_FILES    := $(addprefix ll/,$(notdir $(SOURCE_FILES:.cpp=.ll)))
BIN_FILES    := $(addprefix bin/,$(basename $(notdir $(ASM_FILES))))


all: $(BIN_FILES)
llvmasm: $(ASM_FILES)
bin: $(BIN_FILES)

ll/%.ll: cc/%.cpp
	$(CLANG) -g -emit-llvm -S $< -o - | $(OPT) -mem2reg -S -o $@

bin/%: ll/%.ll
	$(PROFILER) $< -o $@

clean:
	$(RM) -f csv/* bin/*

veryclean:
	$(RM) -f csv/* bin/* ll/*

