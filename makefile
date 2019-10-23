main: main.o BTree.o
	g++ main.o BTree.o -o main

BTree.o: struct.h BTree.h BTree.cpp
	g++ -g  -c BTree.cpp 

main.o: main.cpp BTree.h
	g++ -g  -c main.cpp 

clean:
	rm  main.o BTree.o main
