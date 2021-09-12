#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char** argv) {
	
	int D, lo, hi, t, T, S;

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S)) {    //read arguments from command line
		return -1;
	}


	return 0;
}

