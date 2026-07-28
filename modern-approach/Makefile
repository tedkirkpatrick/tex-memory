#-*- Makefile-*-
CXX=g++
STD=-std=c++23
BASIC_CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
DEBUG_FLAGS=-g -O0
LFLAGS=
COVERAGE_FLAGS=--coverage
LIB_INCLUDE_DIRS=-I../include

ifeq ($(COV),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
	LFLAGS=--coverage
else
	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
endif

EXE=test_box_classes
OBJ=box_classes.o

all: $(EXE)
	@echo "Rebuilt all"

test_box_classes: $(OBJ) test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_box_classes $(OBJ) test_main.o
