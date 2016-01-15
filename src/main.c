/*
 * -*- coding: utf-8 -*-
 *
 * Program:     main
 * Author:      True Merrill
 * Date:        Jan 13 2016
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ising.h"

#define NROWS 20
#define NCOLS 20


int main (void)
{
	ising_t *im;
	ising_problem_t ip;
	double energy, mag, temp;
	size_t i;

	srand(time(NULL));
	im = ising_alloc(NROWS, NCOLS);
	ising_randomize(im);

	// Set model parameters
	ip.H = 0;
	ip.J = 1;
	ip.beta = 5;

	//printf("step  energy  mag  temp\n");
	for (i = 0; i < 3E4; i++) {
		energy = ising_energy(im, &ip);
		mag = ising_magnetization(im, &ip);
		temp = 1 / ip.beta;
		printf("%lu  %e  %e  %e\n", i, energy, mag, temp);
		ising_metropolis(im, &ip);
	}
	
	ising_free(im);
	return 0;
}
