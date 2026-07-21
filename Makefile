CXX=g++
STD=-std=c++23
CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
LFLAGS=

EXE=test_boxes test_memory
OBJ=reporting_errors.o basic_memory.o
BOX_OBJ=boxes.o destroying_boxes.o display.o eqtb.o hash.o printing.o \
	scaled.o string_handling.o token_list.o

all: $(EXE)
	@echo "Rebuilt all"

test_memory: $(OBJ) test_memory.o test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_memory $(OBJ) test_memory.o test_main.o

test_boxes: $(OBJ) $(BOX_OBJ) test_boxes.o test_main.o
	$(CXX) $(STD) $(LFLAGS) -o test_boxes $(OBJ) $(BOX_OBJ) test_boxes.o test_main.o

clean:
	/bin/rm -f $(EXE) basic_tests.o test_main.o $(OBJ) $(BOX_OBJ)
