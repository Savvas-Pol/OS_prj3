all: main

main: main.o helpers.o
	gcc main.o helpers.o -o main

main.o: main.c
	gcc -c main.c
	
helpers.o: helpers.c
	gcc -c helpers.c

clean:
	rm -f main main.o helpers.o
