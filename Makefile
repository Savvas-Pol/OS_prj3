OBJS = main.o helpers.o list.o
SOURCE = main.c helpers.c list.c
HEADERS = helpers.h list.h
OUT = main
CC = gcc
FLAGS = -g -c -Wall -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.c
	$(CC) $(FLAGS) main.c
	
helpers.o: helpers.c
	$(CC) $(FLAGS) helpers.c

list.o: list.c
	$(CC) $(FLAGS) list.c
	
clean:
	rm -f $(OBJS) $(OUT)
