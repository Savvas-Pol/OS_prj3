#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "helpers.h"
#include "list.h"



int main(int argc, char** argv) {
	
	srand(time(0));

	int i, D, lo, hi, t, T, S;
	int flag = 1, size, nextArrival, duration;
	int arrival;
	char* alg;

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}
	
	List* spaceList = list_init();		//list for memory spaces
	list_insert(spaceList, 0, S-1);
	List* usedList = list_init();		//list for used memory spaces
	List* L = list_init();				//list for waiting processes

	for(i = 0; i < D; i++) {

		if(i == 0) {	//beginning of simulation
			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);
		} else if(nextArrival == 0) {	//arrival of process

			//place in lists
			if(strcmp(alg, "best-fit") == 0) {


			} else if(strcmp(alg, "worst-fit") == 0) {


			} else {	//buddy

			}

			nextArrival = generatePoissonVariable(t);
			size = generateUniformVariable(lo, hi);
			//duration = (int)generateExponentialVariable((double)T);
			duration = generatePoissonVariable(T);
		}

		nextArrival--;

	}

	free(alg);
	
	return 0;
}

