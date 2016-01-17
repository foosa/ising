/*
 * -*- coding: utf-8 -*-
 *
 * Program:     ising
 * Author:      True Merrill
 * Date:        Jan 13 2016
 *
 */


#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "ising.h"
#include "log.h"


ising_t * ising_alloc (size_t rows, size_t cols, double H, double J,
	double beta)
{
	ising_t *im;

	im = malloc(sizeof(ising_t));
	im->rows = rows;
	im->cols = cols;
	im->size = rows * cols;
	im->data = malloc(sizeof(int) * im->size);

	// Set default values ...	
	im->J = J;
	im->H = H;
	im->beta = beta;

	// Randomize spins ...
	ising_randomize(im);

	return im;
}


void ising_free (ising_t *im)
{
	free(im->data);
	free(im);
}


/**
 * @brief Wrap the index to enforce circular boundary conditions
 *
 * @param max maximum value of the index
 * @param index index to wrap
 * @return wrapped index
 */
size_t index_wrap (int max, int index)
{
	size_t idx;
	while (index < 0) {
		index += max;
	}
	while (index >= max) {
		index -= max;
	}

	// Assert index is in bounds
	assert(index < max);
	assert(index >= 0);
	idx = (size_t) index;
	return idx;
}


size_t ising_index (const ising_t *im, int row, int col)
{
	size_t index;

	// Enforce circular boundary conditions
	row = index_wrap(im->rows, row);
	col = index_wrap(im->cols, col);
	index = row + im->rows * col;

	// Assert index is in bounds
	assert(index < im->size);
	assert(index >= 0);

	return index;
}


void ising_set (ising_t *im, int row, int col, int spin)
{
	size_t index;
	double delta;

	if ((spin == 1) ||  (spin == -1)) {
		index = ising_index(im, row, col);
		
		// Update the energy and mag parameters
		if (im->data[index] != spin) {
			delta = ising_delta(im, row, col);
			im->energy += delta;
			im->mag += 2 * spin;
			im->data[index] = spin;
		}

		// Otherwise the spin remains the same
	}
	else {
		error("Invalid spin value %d", spin);
	}
}


void ising_flip (ising_t *im, int row, int col)
{
	int spin = ising_get(im, row, col);
	ising_set(im, row, col, - spin);
}


int ising_get (const ising_t *im, int row, int col)
{
	int spin;
	size_t index = ising_index(im, row, col);
	spin = im->data[index];
	return spin;
}


int ising_neighbor (const ising_t *im, int row, int col)
{
	int sum = ising_get(im, row, col + 1) + \
			  ising_get(im, row, col - 1) + \
			  ising_get(im, row + 1, col) + \
			  ising_get(im, row - 1, col);
	return sum;
}


/**
 * @brief Calculates the lattice energy and magnetization and stores the
 * resultant in im->energy and im->mag.  This function iterates over all
 * lattice sites, and should not be used frequently to avoid overhead issues.
 *
 * @param im pointer to an Ising lattice
 */
void ising_calculate (ising_t *im)
{
	int i, j, mag;
	double energy;

	energy = 0;
	mag = 0;

	for (i = 0; i < im->rows; i++) {
		for (j = 0; j < im->cols; j++) {
			energy += ising_site_energy(im, i, j);
			mag += ising_get(im, i, j);
		}
	}

	im->energy = energy;
	im->mag = mag;
}


void ising_randomize (ising_t *im)
{
	double p;
	size_t i;

	for (i = 0; i < im->size; i++) {
		p = ((double) rand()) / RAND_MAX;

		if (p > 0.5) {
			im->data[i] = 1;
		}

		else {
			im->data[i] = -1;
		}
	}

	ising_calculate(im);
}


double ising_site_energy (const ising_t *im, int row, int col)
{
	int spin, neighbors;
	double energy;

	spin = ising_get(im, row, col);
	neighbors = ising_neighbor(im, row, col);
	energy = - (im->H * spin) - (im->J * (spin * neighbors));

	return energy;
}


double ising_delta (const ising_t *im, int row, int col)
{
	int spin, neighbors;
	double e_start, e_finish, delta;

	spin = ising_get(im, row, col);
	neighbors = ising_neighbor(im, row, col);
	e_start = - (im->H * spin) - (im->J * (spin * neighbors));
	spin *= -1;
	e_finish = - (im->H * spin) - (im->J * (spin * neighbors));
	delta = e_finish - e_start;
	
	return delta;
}


double ising_energy (const ising_t *im)
{
	return im->energy;
}


int ising_magnetization (const ising_t *im)
{
	return im->mag;
}


void ising_metropolis_step (ising_t *im, int row, int col)
{
	double delta, r, p;

	// Calculate the energy from flipping the site
	delta = ising_delta(im, row, col);

	if (delta >= 0) {
		p = exp(-im->beta * delta);
		r = ((double) rand()) / RAND_MAX;
		
		if (r < p) {
			// Accept spin flip
			ising_flip(im, row, col);
		}
	}
	
	else {
		// Accept spin flip
		ising_flip(im, row, col);
	}
}


void ising_metropolis (ising_t *im)
{
	int row, col;
	row = rand() % im->rows;
	col = rand() % im->cols;
	ising_metropolis_step(im, row, col);
}

