#ifndef HELPERS_H
#define HELPERS_H

int readArguments(int argc, char** argv, int* D, int* lo, int* hi, int* t, int* T, int* S, char** alg); //read arguments from command line
int generatePoissonVariable(int t); //calculation with Poisson distribution
int generateUniformVariable(int lo, int hi); //calculation with uniform distribution
double generateExponentialVariable(double T); //calculation with exponential distribution

#endif