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
#include "ising.h"
#include "log.h"


ising_t * ising_alloc (size_t rows, size_t cols)
{
	ising_t *im;
	size_t i;

	im = malloc(sizeof(ising_t));
	im->rows = rows;
	im->cols = cols;
	im->size = rows * cols;
	im->data = malloc(sizeof(int) * im->size);

	// Set all of the spins to be spin down ...
	for (i = 0; i < im->size; i++) {
		im->data[i] = -1;
	}

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
	while (index < 0) {
		index += max;
	}
	while (index > max) {
		index -= max;
	}
	return index;
}


size_t ising_index (const ising_t *im, int row, int col)
{
	size_t index;

	// Enforce circular boundary conditions
	row = index_wrap(im->rows, row);
	col = index_wrap(im->cols, col);
	index = row + im->rows * col;

	return index;
}


void ising_set (ising_t *im, int row, int col, int spin)
{
	size_t index;

	if ((spin == 1) ||  (spin == -1)) {
		index = ising_index(im, row, col);
		im->data[index] = spin;
	}
	else {
		error("Invalid spin value %d", spin);
	}
}


void ising_flip (ising_t *im, int row, int col)
{
	size_t index = ising_index(im, row, col);
	im->data[index] *= -1;
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


void ising_randomize (ising_t *im)
{
	double p;
	size_t i, j;

	for (i = 0; i < im->rows; i++) {
		for (j = 0; j < im->cols; j++) {
			p = ((double) rand()) / RAND_MAX;
			if (p > 0.5) {
				ising_flip(im, i, j);
			}
		}
	}
}

/*
ising_problem_t * ising_problem_alloc (size_t history)
{
	ising_problem_t *ip;
	ip = malloc(sizeof(ising_problem_t));

	// Allocate storage for history only if the user asked us for it
	if (history > 0) {
		ip->E = malloc(sizeof(double) * history);
	}
	else {
		ip-> = NULL;
	}

	ip->J = 0;
	ip->H = 0;
	ip->beta = 0;
	ip->history = history;
	return ip;
}


void ising_problem_free (ising_problem_t *ip)
{
	free(ip->E);
	free(ip);
}
*/

double ising_site_energy (const ising_t *im, const ising_problem_t *ip,
	int row, int col)
{
	int spin, neighbors;
	double energy;

	spin = ising_get(im, row, col);
	neighbors = ising_neighbor(im, row, col);
	energy = - ip->H * spin - ip->J * (spin * neighbors);
	return energy;
}


double ising_energy (const ising_t *im, const ising_problem_t *ip)
{
	int i, j;
	double energy;

	energy = 0;
	for (i = 0; i < im->rows; i++) {
		for (j = 0; j < im->cols; j++) {
			energy += ising_site_energy(im, ip, i, j);
		}
	}

	energy = energy / (double) im->size;
	return energy;
}


double ising_magnetization (const ising_t *im, const ising_problem_t *ip)
{
	int i, j;
	double mag;

	mag = 0;
	for (i = 0; i < im->rows; i++) {
		for (j = 0; j < im->cols; j++) {
			mag += ising_get(im, i, j);
		}
	}

	mag = mag / im->size;
	return mag;
}


void ising_metropolis_step (ising_t *im, const ising_problem_t *ip,
	int row, int col)
{
	double e_start, e_finish, delta, r, p;
	
	// Calculate the energy from flipping the site
	e_start = ising_site_energy(im, ip, row, col);
	ising_flip(im, row, col);
	e_finish = ising_site_energy(im, ip, row, col);
	delta = e_finish - e_start;

	if (delta > 0) {
		p = exp(-ip->beta * delta);
		r = ((double) rand()) / RAND_MAX;
		if (r > p) {
			// Reject spin flip
			ising_flip(im, row, col);
		}
	}
}


void ising_metropolis (ising_t *im, const ising_problem_t *ip)
{
	int row, col;
	row = rand() % im->rows;
	col = rand() % im->cols;
	ising_metropolis_step(im, ip, row, col);
}

/*
int ising_main (const char *path)
{
	ising_t *im;
	ising_problem_t = ip;
	json_t *root, *rows, *cols, *J, *H, *beta;
	json_error_t err;

	// Parse the input file
	root = json_load_file(path, 0, &err);
	rows = json_object_get(root, "rows");
	cols = json_object_get(root, "cols");
	im = ising_alloc(json_integer_value(rows), json_integer_value(cols));

	// These are the scannable variables.  They each have to be arrays...
	J = json_object_get(root, "J");
	H = json_object_get(root, "H");
	beta = json_object_get(root, "beta");

	

	// Free JSON related data structures
	json_decref(beta);
	json_decref(H);
	json_decref(J);
	json_decref(cols);
	json_decref(rows);
	json_decref(root);
}
*/
