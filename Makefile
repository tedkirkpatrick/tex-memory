#-*- Makefile-*-
CXX=g++
STD=-std=c++23
BASIC_CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
DEBUG_FLAGS=-g -O0
LFLAGS=
COVERAGE_FLAGS=--coverage
LIB_INCLUDE_DIRS=-Iinclude

ifeq ($(COV),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
	LFLAGS=--coverage
else
	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
endif


EXE=test_boxes test_destroy test_memory
MEMORY_OBJ=basic_memory.o reporting_errors.o
BOX_OBJ=boxes.o destroying_boxes.o display.o eqtb.o hash.o printing.o \
	scaled.o string_handling.o token_list.o
SAMPLE_LISTS=sample_lists.o

all: $(EXE)
	@echo "Rebuilt all"

test_boxes: $(MEMORY_OBJ) $(BOX_OBJ) test_boxes.o test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_boxes $(MEMORY_OBJ) $(BOX_OBJ) test_boxes.o test_main.o

test_destroy: $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) test_destroy.o test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_destroy $(MEMORY_OBJ) $(BOX_OBJ) $(SAMPLE_LISTS) test_destroy.o test_main.o

test_memory: $(MEMORY_OBJ) test_memory.o test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_memory $(MEMORY_OBJ) test_memory.o test_main.o

clean:
	/bin/rm -f $(EXE) test_memory.o test_boxes.o test_main.o $(MEMORY_OBJ) $(BOX_OBJ)

test: $(EXE)
	./test_memory
	./test_boxes
	./test_destroy

