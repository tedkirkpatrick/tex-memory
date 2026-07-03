CXX=g++
STD=-std=c++23
CPPFLAGS=$(STD) -Wall -Wpedantic -Werror
LFLAGS=

OBJ=basic-memory.o overflow.o

basic-memory: $(OBJ) basic-tests.o test-main.o
	$(CXX) $(STD) $(LFLAGS) -o basic-memory $(OBJ) basic-tests.o test-main.o

test-boxes: $(OBJ) test-boxes.o test-main.o
	$(CXX) $(STD) $(LFLAGS) -o test-boxes $(OBJ) test-boxes.o test-main.o

clean:
	/bin/rm -f basic-memory basic-tests.o test-main.o $(OBJ)
