#-*- Makefile-*-
CXX=g++
STD=-std=c++23
BASIC_CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
DEBUG_FLAGS=-g
OPT_LEVEL=-O0
DEFINED_SYMBOLS=-D OPT_LEVEL=\"$(OPT_LEVEL)\"
LFLAGS=
COVERAGE_FLAGS=--coverage
LIB_INCLUDE_DIRS=-I../include -I../common
ASM_OPT=$(DEBUG_FLAGS)

TRAVERSALS=1

ifeq ($(COV),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS)
	LFLAGS=--coverage
else
	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS)
endif

EXE=run_benchmarks test_boxes test_destroy test_memory
MEMORY_OBJ=basic_memory.o reporting_errors.o
BOX_OBJ=boxes.o destroying_boxes.o display.o eqtb.o hash.o printing.o \
	scaled.o string_handling.o token_list.o
SAMPLE_LISTS=sample_lists.o traverse.o
BENCHMARK_UTILS=../common/benchmark_utils.o
BENCHMARKS=benchmarks.o $(BENCHMARK_UTILS)
BENCHMARK_RESULTS_FILE=../benchmarks.csv
TEST_MAIN=../common/test_main.o

%.s: %.cpp
	$(CXX) $(STD) $(BASIC_CPPFLAGS) $(OPT_LEVEL) $(LIB_INCLUDE_DIRS) $(DEFINED_SYMBOLS) $(ASM_OPT) -S -fno-verbose-asm $< -o $@

all: $(EXE)
	@echo "Rebuilt all"

run_benchmarks: $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) $(BENCHMARKS)
	$(CXX) $(STD) $(LFLAGS) -o run_benchmarks $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) $(BENCHMARKS) $(TEST_MAIN)

test_boxes: $(MEMORY_OBJ) $(BOX_OBJ) test_boxes.o $(TEST_MAIN)
	$(CXX) $(STD) $(LFLAGS) -o test_boxes $(MEMORY_OBJ) $(BOX_OBJ) test_boxes.o $(BENCHMARK_UTILS) $(TEST_MAIN)

test_destroy: $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) test_destroy.o
	$(CXX) $(STD) $(LFLAGS) -o test_destroy $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) test_destroy.o $(BENCHMARK_UTILS) $(TEST_MAIN)

test_memory: $(MEMORY_OBJ) test_memory.o
	$(CXX) $(STD) $(LFLAGS) -o test_memory $(MEMORY_OBJ) test_memory.o $(BENCHMARK_UTILS) $(TEST_MAIN)

clean:
	/bin/rm -f $(EXE) benchmarks.o test_boxes.o test_destroy.o test_memory.o  $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) benchmarks.o

test: all
	./test_memory
	./test_boxes
	./test_destroy

# Fail at start of benchmark if output file has not been created
exist: $(BENCHMARK_RESULTS_FILE)

benchmark: exist clean test
	./run_benchmarks --traversals $(TRAVERSALS) -r xml > benchmark.xml
	../parse-benchmarks.py --input_filename benchmark.xml $(BENCHMARK_RESULTS_FILE)
