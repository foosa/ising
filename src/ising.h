/*
 * -*- coding: utf-8 -*-
 *
 * Program:     ising
 * Author:      True Merrill
 * Date:        Jan 13 2016
 *
 */


#ifndef ISING_H
#define ISING_H


typedef struct 
{
	size_t rows;
	size_t cols;
	size_t size;
	int *data;
} ising_t;


typedef struct
{
	double J;
	double H;
	double beta;
} ising_problem_t;


ising_t * ising_alloc (size_t rows, size_t cols);
void ising_free (ising_t *im);
void ising_set (ising_t *im, int row, int col, int spin);
void ising_flip (ising_t *im, int row, int col);
int ising_get (const ising_t *im, int row, int col);
int ising_neighbor (const ising_t *im, int row, int col);
void ising_randomize (ising_t *im);


double ising_site_energy (const ising_t *im, const ising_problem_t *ip, int row, int col);
double ising_energy (const ising_t *im, const ising_problem_t *ip);
double ising_magnetization (const ising_t *im, const ising_problem_t *ip);


void ising_metropolis_step (ising_t *im, const ising_problem_t *ip, int row, int col);
void ising_metropolis (ising_t *im, const ising_problem_t *ip);


#endif /* end of include guard: ISING_H */
