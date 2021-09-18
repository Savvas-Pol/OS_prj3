#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "helpers.h"
#include "usedList.h"
#include "spaceList.h"
#include "list.h"

int main(int argc, char** argv) {
	
	srand(time(0));

	int i, D, lo, hi, t, T, S;
	int size, nextArrival, duration;
	char* alg;

	if (readArguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}

	SpaceList* spaceList = spaceList_init();		//list for memory spaces
	spaceList_insert(spaceList, 0, S-1);
	UsedList* usedList = usedList_init();		//list for used memory spaces
	List* L = list_init();				//list for waiting processes
	SpaceListNode* spaceListSpace;

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
					spaceListSpace->start = spaceListSpace->start + size;
				} else {
					list_insert(L, size, duration); //if it doesn't fit in available spaces, then place in L list
				}
			} else {
				list_insert(L, size, duration);	//if memory is full, then place in L list
			}

			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);
			
		}

		usedList_reduceDurations(usedList);				//reduce duration of processes on every iteration
		checkFinishedProcesses(usedList, spaceList);		//check if any of the processes has finished
		checkWaitingList(L, spaceList, usedList, alg);		//check waiting list
		nextArrival--;
	}

	free(alg);
	
	return 0;
}
