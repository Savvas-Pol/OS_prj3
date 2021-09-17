#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "helpers.h"


int read_arguments(int argc, char** argv, int* D, int* lo, int* hi, int* t, int* T, int* S, char** alg) {    //read arguments from command line

	int i;

	if (argc != 15) {
		printf("Wrong arguments!!!\n");
		return -1;
	} else {
		for (i = 0; i < 15; i++) {					//check flags and assign the next value to the correct variable
			if (!strcmp(argv[i], "-D")) {
				*D = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-lo")) {
				*lo = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-hi")) {
				*hi = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-t")) {
				*t = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-T")) {
				*T = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-S")) {
				*S = atoi(argv[i + 1]);
			} else if (!strcmp(argv[i], "-alg")) {
				*alg = malloc(strlen(argv[i + 1]) +1);
				strcpy(*alg, argv[i + 1]);
			}
		}
	}

	return 0;
}

int generatePoissonVariable(int t) {						//calculation with Poisson distribution

	int k = 0;
	long double p = 1.0;
	long double l = exp (-t);

	while (p >= l) {
		double u = rand()/(double)RAND_MAX;
		p *= u;
		k ++;
	}

	return k-1;
}

int generateUniformVariable(int lo, int hi) {				//calculation with uniform distribution
	return rand()%(hi-lo+1) + lo;
}

double generateExponentialVariable(double T) {				//calculation with exponential distribution
	double u;
    u = rand() / (RAND_MAX + 2.0);
    return -log(1 - u) / T;
}

