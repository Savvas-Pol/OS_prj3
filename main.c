#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "helpers.h"
#include "usedList.h"
#include "spaceList.h"
#include "list.h"

int main(int argc, char** argv) {
	
	srand(time(0));

	int i, D, lo, hi, t, T, S;
	int size, nextArrival, duration;
	char* alg;

	FILE* logfile = fopen("logfile", "w");

	if (readArguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}

	SpaceList* spaceList = spaceList_init();		//list for memory spaces
	spaceList_insert(spaceList, 0, S-1);
	UsedList* usedList = usedList_init();		//list for used memory spaces
	List* L = list_init();				//list for waiting processes
	SpaceListNode* spaceListSpace;

	//pid_t pid = fork();

	//shared memory segment

	for(i = 0; i < D; i++) {

		if(i == 0) {	//beginning of simulation
			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);

		} else if(nextArrival == 0){	//arrival of process

			if((spaceList_full(spaceList) == 0)) {

				if(strcmp(alg, "best-fit") == 0) {
					spaceListSpace = bestFit(spaceList, size);
				} else if(strcmp(alg, "worst-fit") == 0) {
					spaceListSpace = worstFit(spaceList, size);
				} else {	//buddy
					spaceListSpace = buddy(spaceList, size);
				}

				if(spaceListSpace != NULL) {
					usedList_insert(usedList, spaceListSpace->start, spaceListSpace->start + size - 1, duration);
					fprintf(logfile, "Process with id: %d placed in memory from: %d to: %d at: %d time\n", usedList->count-1, spaceListSpace->start, spaceListSpace->start + size - 1, i);
					spaceListSpace->start = spaceListSpace->start + size;
				} else {
					list_insert(L, size, duration, i); //if it doesn't fit in available spaces, then place in L list
					fprintf(logfile, "Process with size: %d and duration: %d can't fit in available memory spaces! Placed in waiting list at: %d time\n", size, duration, i);
				}
			} else {
				list_insert(L, size, duration, i);	//if memory is full, then place in L list
				fprintf(logfile, "Memory full! Process with size: %d and duration: %d placed in waiting list at: %d time\n", size, duration, i);
			}

			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);
			
		}

		usedList_reduceDurations(usedList);				//reduce duration of processes on every iteration
		checkFinishedProcesses(usedList, spaceList, logfile, i);		//check if any of the processes has finished
		checkWaitingList(L, spaceList, usedList, alg, logfile, i);		//check waiting list
		nextArrival--;
	}

	fprintf(logfile, "Totally %d processes were placed in waiting list\n", L->count);

	//metrics

	fclose(logfile);

	free(alg);
	
	return 0;
}
