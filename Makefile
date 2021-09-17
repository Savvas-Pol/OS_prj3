OBJS = main.o helpers.o usedList.o spaceList.o
SOURCE = main.c helpers.c usedList.c spaceList.c
HEADERS = helpers.h usedList.h spaceList.h
OUT = main
CC = gcc
FLAGS = -g -c -Wall
LFLAGS = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c
	
helpers.o: helpers.c
	$(CC) $(FLAGS) helpers.c

usedList.o: usedList.c
	$(CC) $(FLAGS) usedList.c
	
spaceList.o: spaceList.c
	$(CC) $(FLAGS) spaceList.c
	
clean:
	rm -f $(OBJS) $(OUT)
