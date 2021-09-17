#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "helpers.h"
#include "usedList.h"
#include "spaceList.h"


int main(int argc, char** argv) {
	
	srand(time(0));

	int i, D, lo, hi, t, T, S;
	int flag = 1, size, nextArrival, duration;
	int arrival;
	char* alg;

	SpaceListNode* new_process;

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}

	SpaceList* spaceList = spaceList_init();		//list for memory spaces
	spaceList_insert(spaceList, 0, S-1);
	UsedList* usedList = usedList_init();		//list for used memory spaces
	UsedList* L = usedList_init();				//list for waiting processes

	for(i = 0; i < D; i++) {

		if(i == 0) {	//beginning of simulation
			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);

		} else if(nextArrival == 0){	//arrival of process

			if((spaceList_full(spaceList) == 0) && ((new_process = spaceList_findBestFit(spaceList, size)) != NULL)) {
				if(strcmp(alg, "best-fit") == 0) {
					usedList_insert(usedList, new_process->start, new_process->start + size - 1, duration);
					new_process->start = new_process->start + size;

					// spaceList_print(spaceList);
					// usedList_print(usedList);

				} else if(strcmp(alg, "worst-fit") == 0) {


				} else {	//buddy

				}
			} else {
				//place in L list
			}

			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);
			
		}
		
		usedList_reduceDurations(usedList);
		check_finished_processes(usedList, spaceList);
		printf("%d. Size: %d\n",i, size);
		nextArrival--;

	}

	usedList_print(usedList);
	free(alg);
	
	return 0;
}

