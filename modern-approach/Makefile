#-*- Makefile-*-
CXX=g++
STD=-std=c++23
BASIC_CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
DEBUG_FLAGS=-g -O0
LFLAGS=
COVERAGE_FLAGS=--coverage
SANITIZER_FLAGS=-fsanitize=leak
LIB_INCLUDE_DIRS=-I../include

ifeq ($(COV),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
	LFLAGS=--coverage
else
	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
endif

ifeq ($(SAN),yes)
	CPPFLAGS=$(BASIC_CPPFLAGS) $(SANITIZER_FLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
	LFLAGS=-fsanitize=leak
else
	CPPFLAGS=$(BASIC_CPPFLAGS) $(DEBUG_FLAGS) $(LIB_INCLUDE_DIRS)
endif

EXE=test_box_classes
OBJ=box_classes.o box_types.o

all: $(EXE)
	@echo "Rebuilt all"

box_classes.o: box_classes.cpp box_classes.hpp
	$(CXX) $(STD) $(CPPFLAGS) -c box_classes.cpp

test_box_classes: $(OBJ) test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_box_classes $(OBJ) test_main.o

clean:
	/bin/rm -f $(EXE) $(OBJ) test_main.o
