all: main

bb.o: bb.cpp bb.hpp
	g++ -c bb.cpp
struct.o: struct.cpp
	g++ -c struct.cpp
main.o: main.cpp bb.cpp bb.hpp struct.cpp
	g++ -c main.cpp
main: bb.o struct.o main.o
	g++ bb.o struct.o main.o -o main -lfltk
clean:
	rm -rf *.o
	rm -rf main

