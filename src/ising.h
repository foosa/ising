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

#define TRUE 1
#define FALSE 0


/**
 * @brief Type describing a 2D Ising model
 *
 * Users should not access the ENERGY, MAG, and CURRENT fields
 */
typedef struct 
{
	size_t rows;	///< Number of rows in the 2D lattice 
	size_t cols;	///< Number of columns in the 2D lattice
	size_t size;	///< Number of spins in the 2D lattice
	int *data;		///< Pointer to array containing raw spin data
	double J;		///< Spin-spin coupling parameter
	double H;		///< Magnetic field parameter		
	double beta;	///< Inverse temperature parameter
	double energy;	///< Total lattice energy (DO NOT MODIFY)
	int mag;		///< Total lattice magnetization (DO NOT MODIFY)
} ising_t;


/**
 * @brief Allocate an Ising model
 *
 * @param rows number of lattice rows
 * @param cols number of lattice columns
 * @param H magnetic field parameter
 * @param J spin-spin coupling parameter
 * @param beta inverse temperature parameter
 *
 * @return pointer to newly allocated Ising lattice
 */
ising_t * ising_alloc (size_t rows, size_t cols, double H, double J,
	double beta);


/**
 * @brief Free memory allocated to an Ising lattice
 *
 * @param im pointer to an Ising lattice
 */
void ising_free (ising_t *im);


/**
 * @brief Set the value of spin at site `(row, col)` to `spin`
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 * @param spin spin value.  Must be 1 or -1.
 */
void ising_set (ising_t *im, int row, int col, int spin);


/**
 * @brief Flip the spin at site `(row, col)`
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 */
void ising_flip (ising_t *im, int row, int col);


/**
 * @brief Get the value of the spin at site `(row, col)`
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 *
 * @return value of the spin
 */
int ising_get (const ising_t *im, int row, int col);


/**
 * @brief Get the sum of the spins of the four neighboring lattice sites
 * 	adjacent to `(row, col)`
 *
 * @param im ponter to an Ising lattice
 * @param row row index
 * @param col column index
 *
 * @return sum of the neighboring spins
 */
int ising_neighbor (const ising_t *im, int row, int col);


/**
 * @brief Initialize the Ising lattice into a randomized spin state
 *
 * @param im pointer to an Ising lattice
 */
void ising_randomize (ising_t *im);


/**
 * @brief Calculate the energy at a site in an Ising lattice
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 *
 * @return site energy
 */
double ising_site_energy (const ising_t *im, int row, int col);


/**
 * @brief Calculate the change in energy when flipping the orientation of the
 *  spin at `(row, col)`.
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 *
 * @return change in energy
 */
double ising_delta (const ising_t *im, int row, int col);


/**
 * @brief Returns the energy across the entire Ising lattice.
 *
 * @param im pointer to an Ising lattice
 *
 * @return lattice energy
 */
double ising_energy (const ising_t *im);


/**
 * @brief Returns th magnetization across the entire Ising lattice.
 *
 * @param im pointer to an Ising lattice
 *
 * @return lattice magnetization
 */
int ising_magnetization (const ising_t *im);


/**
 * @brief Update a single lattice site according to the Metropolis algorithm
 *
 * @param im pointer to an Ising lattice
 * @param row row index
 * @param col column index
 */
void ising_metropolis_step (ising_t *im, int row, int col);


/**
 * @brief Update a random lattice site according to the Metropolis algorithm
 *
 * @param im pointer to an Ising lattice
 */
void ising_metropolis (ising_t *im);


#endif /* end of include guard: ISING_H */

