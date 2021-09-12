#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char** argv) {
	
	int D, lo, hi, t, T, S;

	// if (argc != 7) {
	// 	printf("Wrong arguments!\n");
	// 	return -1;
	// }

	if (read_arguments(argc, argv, &D, &lo, &hi, &t, &T, &S)) {    //read arguments from command line
		return -1;
	}

	// D = atoi(argv[1]);
	// lo = atoi(argv[2]);
	// hi = atoi(argv[3]);
	// t = atoi(argv[4]);
	// T = atoi(argv[5]);
	// S = atoi(argv[6]);


	printf("D = %d\nlo = %d\nhi = %d\nt = %d\nT = %d\nS = %d\n", D, lo, hi, t, T, S);

	return 0;
}

