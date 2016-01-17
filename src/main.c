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


void ising_print (ising_t *im)
{
	size_t i, j;

	for (i = 0; i < im->rows; i++) {
		for (j = 0; j < im->cols; j++) {
			printf("%+i", ising_get(im, i, j));
		}
		printf("\n");
	}
}


int main (void)
{
	ising_t *im;
	double energy, mag, temp;
	size_t i;

	srand(time(NULL));
	im = ising_alloc(NROWS, NCOLS, 1, 0, 1E5);

	//printf("step  energy  mag  temp\n");
	for (i = 0; i < 1E4; i++) {
		energy = ising_energy(im);
		mag = ising_magnetization(im);
		temp = 1 / im->beta;
		printf("%lu  %e  %e  %e\n", i, energy, mag, temp);
		ising_metropolis(im);
	}

	ising_free(im);
	return 0;
}

