#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "list.h"

int main(int argc, char** argv) {
	
	int D, lo, hi, t, T, S;
	char* alg;

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S, &alg)) {    //read arguments from command line
		return -1;
	}

	List* l = list_init();

	list_destroy(l);

	free(alg);
	
	return 0;
}

