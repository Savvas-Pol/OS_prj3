OBJS = main.o helpers.o usedList.o spaceList.o list.o shared_memory.o main_g.o main_m.o
SOURCE = main.c helpers.c usedList.c spaceList.c list.c shared_memory.c main_g.c main_m.c
HEADERS = helpers.h usedList.h spaceList.h list.h shared_memory.h
OUT = main
CC = gcc
FLAGS = -g -c -Wall
LFLAGS = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c
	
main_g.o: main_g.c
	$(CC) $(FLAGS) main_g.c
	
main_m.o: main_m.c
	$(CC) $(FLAGS) main_m.c
	
shared_memory.o: shared_memory.c
	$(CC) $(FLAGS) shared_memory.c
	
helpers.o: helpers.c
	$(CC) $(FLAGS) helpers.c

usedList.o: usedList.c
	$(CC) $(FLAGS) usedList.c
	
spaceList.o: spaceList.c
	$(CC) $(FLAGS) spaceList.c
	
list.o: list.c
	$(CC) $(FLAGS) list.c
	
clean:
	rm -f $(OBJS) $(OUT)
