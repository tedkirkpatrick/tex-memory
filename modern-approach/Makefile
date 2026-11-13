#-*- Makefile-*-
# Build the modern version of the TeX data structures and run benchmarks.
# See ../README.md for instructions of use.
#
# This code is in the public domain:
# See https://creativecommons.org/publicdomain/zero/1.0/
#
CXX=g++
STD=-std=c++23
# Turn off errors for unused functions and variables so we can have unused debugging functions
BASIC_CPPFLAGS=$(STD) -Wall -Wpedantic -Werror -Wno-unused-function -Wno-unused-variable
DEBUG_FLAGS=-g
OPT_LEVEL=-O0
DEFINED_SYMBOLS=-D OPT_LEVEL=\"$(OPT_LEVEL)\"
LFLAGS=
COVERAGE_FLAGS=--coverage
SANITIZER_FLAGS=-fsanitize=leak
LIB_INCLUDE_DIRS=-I../include -I../common
ASM_OPT=$(DEBUG_FLAGS)

BENCHMARK_UTILS=../common/benchmark_utils.o
BENCHMARK_RESULTS_FILE=../benchmarks.csv
TEST_MAIN=../common/test_main.o

TRAVERSALS=1

ifeq ($(COV),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS)
	LFLAGS=--coverage
else ifeq ($(SAN),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(SANITIZER_FLAGS) $(DEBUG_FLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS)
	LFLAGS=-fsanitize=leak
else

	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS)
endif

EXE=run_benchmarks test_boxes run_variance_benchmark
OBJ=box_classes.o box_types.o sample_lists.o
VAR_OBJ=benchmark_variance.o

%.s: %.cpp
	$(CXX) $(STD) $(BASIC_CPPFLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS) $(ASM_OPT) -S -fno-verbose-asm $< -o $@

all: $(EXE)
	@echo "Rebuilt all"

box_classes.o: box_classes.cpp box_classes.hpp
	$(CXX) $(STD) $(CPPFLAGS) -c box_classes.cpp

run_benchmarks: $(OBJ) benchmarks.o
	$(CXX) $(STD) $(LFLAGS) -o $@ $(OBJ) benchmarks.o $(BENCHMARK_UTILS) $(TEST_MAIN)

run_variance_benchmark: $(VAR_OBJ)
	$(CXX) $(STD) $(LFLAGS) -o $@ $(VAR_OBJ) $(BENCHMARK_UTILS) $(TEST_MAIN)

test_boxes: $(OBJ) test_boxes.o 
	$(CXX) $(STD) $(LFLAGS) -o $@ $(OBJ) test_boxes.o $(BENCHMARK_UTILS) $(TEST_MAIN)

clean:
	/bin/rm -f $(EXE) $(OBJ)

test: $(EXE)
	./test_boxes

 # Fail at start of benchmark if output file has not been created
exist: $(BENCHMARK_RESULTS_FILE)

benchmark: exist clean test
	./run_benchmarks --traversals $(TRAVERSALS) -r xml > benchmark.xml
	../parse-benchmarks.py --input_filename benchmark.xml $(BENCHMARK_RESULTS_FILE)
