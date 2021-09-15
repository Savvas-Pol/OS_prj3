#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "helpers.h"
#include "list.h"

int main(int argc, char** argv) {
	
	srand(time(0));

	int i, D, lo, hi, t, T, S;
	int flag = 1, size, next;
	char* alg;

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}
	
	List* spacelist = list_init();		//list for memory spaces
	list_insert(spacelist, 0, S-1);
	List* usedlist = list_init();		//list for used memory spaces
	List* L = list_init();				//list for waiting processes

	for(i = 0; i < D; i++) {

		size = (rand() % (hi - lo + 1)) + lo;

	}

	free(alg);
	
	return 0;
}

