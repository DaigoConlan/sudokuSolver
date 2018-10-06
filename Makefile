CC = g++
CFLAGS = -g -Wall -Werror

all: build

build: main.o boardio.o solver.o 
	
	$(CC) $(CFLAGS) -o sudokuSolver main.o boardio.o solver.o 

main.o: main.cpp

	$(CC) $(CFLAGS) -c main.cpp

solver.o: solver.cpp

	$(CC) $(CFLAGS) -c solver.cpp

boardio.o: boardio.cpp

	$(CC) $(CFLAGS) -c boardio.cpp

clean:

	rm -f *.o main

rebuild:

	clean all