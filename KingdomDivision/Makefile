bin/kingdom-division: main.o Node.o
	g++ -o bin/kingdom-division main.o Node.o

main.o: main.cpp
	g++ -c main.cpp

Node.o: Node.cpp
	g++ -c Node.cpp

.PHONY: clean

clean: 
	rm -rf main.o Node.o bin/kingdom-division
