#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"


int read_arguments(int argc, char** argv, int* D, int* lo, int* hi, int* t, int* T, int* S) {

	int i;

	if (argc != 13) {
		printf("Wrong arguments!!!\n");
		return -1;
	} else {
		for (i = 0; i < 13; i++) {
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
			}
		}
	}

	return 0;
}