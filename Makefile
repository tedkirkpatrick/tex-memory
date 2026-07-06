CXX=g++
STD=-std=c++23
CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
LFLAGS=

EXE=basic-memory test-boxes
OBJ=basic-memory.o overflow.o
BOX_OBJ=boxes.o eqtb.o print.o

all: $(EXE)
	@echo "Rebuilt all"

basic-memory: $(OBJ) basic-tests.o test-main.o
	$(CXX) $(STD) $(LFLAGS) -o basic-memory $(OBJ) basic-tests.o test-main.o

test-boxes: $(OBJ) $(BOX_OBJ) test-boxes.o test-main.o
	$(CXX) $(STD) $(LFLAGS) -o test-boxes $(OBJ) $(BOX_OBJ) test-boxes.o test-main.o

clean:
	/bin/rm -f $(EXE) basic-tests.o test-main.o $(OBJ) $(BOX_OBJ)
