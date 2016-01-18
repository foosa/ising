/*
 * -*- coding: utf-8 -*-
 *
 * Program:     main
 * Author:      True Merrill
 * Date:        Jan 18 2016
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include "log.h"
#include "ising.h"


struct ising_job_params
{
	size_t rows;
	size_t cols;
	double J;
	double H;
	double beta;
	size_t steps;
};


/**
 * @brief Callback function to be called for every job
 *
 * @param params
 */
void ising_job_callback (void *params)
{
	struct ising_job_params *p;
	ising_t *im;
	size_t step;
	double avg_energy, avg_mag, temp;

	p = (struct ising_job_params *) params;
	im = ising_alloc(p->rows, p->cols, p->H, p->J, p->beta);

	for (step = 0; step < p->steps; step++) {
		avg_energy = im->energy / im->size;
		avg_mag = ((double) im->mag) / im->size;
		temp = 1 / im->beta;
		printf("%lu, %lu, % .10f, % .10f, % .10f\n", step, im->size, avg_energy,
			avg_mag, temp);
		ising_metropolis(im);
	}

	ising_free(im);
}


/**
 * @brief Parse the JSON root object for children
 *
 * @param root root object
 * @param output output object
 * @param field field key for the child
 * @param type JSON type
 */
void parse_input_field (json_t *root, json_t **output, const char *field,
	json_type type)
{
	char *type_name = NULL;

	if (type == JSON_INTEGER) {
		type_name = "integer";
	}
	else if (type == JSON_ARRAY) {
		type_name = "array";
	}
	else if (type == JSON_OBJECT) {
		type_name = "object";
	}

	*output = json_object_get(root, field);
	if (!(*output)) {
		fprintf(stderr, "error: input missing required field %s\n", field);
		json_decref(root);
		exit(-1);
	}

	if (json_typeof(*output) != type) {

		if (type_name) {
			fprintf(stderr, "error: input field %s is not of type %s\n", field,
				type_name);
		}
		else {
			fprintf(stderr, "error: input field %s is of the incorrect type\n",
				field);
		}

		json_decref(root);
		exit(-1);
	}
}


void run (const char *filename)
{
	json_t *root, *rows, *cols, *steps, *H, *J, *beta, *value;
	json_error_t err;
	size_t index;
	struct ising_job_params params;

	// Initialize json_t's to NULL
	root = NULL;
	rows = NULL;
	cols = NULL;
	steps = NULL;
	H = NULL;
	J = NULL;
	beta = NULL;

	// Parse the file and construct the root object
	root = json_load_file(filename, 0, &err);
	if (!root) {
		fprintf(stderr, "error: on line %d: %s\n", err.line, err.text);
		exit(-1);
	}

	if (!json_is_object(root)) {
		fprintf(stderr, "error: not a valid JSON object\n");
		json_decref(root);
		exit(-1);
	}

	// Parse each of the required input fields
	parse_input_field(root, &rows, "rows", JSON_INTEGER);
	parse_input_field(root, &cols, "cols", JSON_INTEGER);
	parse_input_field(root, &H, "H", JSON_ARRAY);
	parse_input_field(root, &J, "J", JSON_ARRAY);
	parse_input_field(root, &beta, "beta", JSON_ARRAY);
	parse_input_field(root, &steps, "steps", JSON_ARRAY);

	params.rows = (size_t) json_integer_value(rows);
	params.cols = (size_t) json_integer_value(cols);

	json_array_foreach (H, index, value) {
		params.H = json_real_value(value);
		json_array_foreach (J, index, value) {
			params.J = json_real_value(value);
			json_array_foreach (beta, index, value) {
				params.beta = json_real_value(value);
				json_array_foreach (steps, index, value) {
					params.steps = (size_t) json_integer_value(value);

					// Run the job
					ising_job_callback(&params);
				}
			}
		}
	}

	json_decref(root);
}


/**
 * @brief Banner printed during program invocation
 */
const char *header = "" 
"    o8o            o8o                         \n"
"    `\"'            `\"'                         \n"
"   oooo   .oooo.o oooo  ooo. .oo.    .oooooooo \n"
"   `888  d88(  \"8 `888  `888P\"Y88b  888' `88b  \n"
"    888  `\"Y88b.   888   888   888  888   888  \n"
"    888  o.  )88b  888   888   888  `88bod8P'  \n"
"   o888o 8\"\"888P' o888o o888o o888o `8oooooo.  \n"
"                                    d\"     YD  \n"
"Copyright (c) 2016 True Merrill     \"Y88888P'  \n"
"All rights reserved                            \n";


/**
 * @brief Main function 
 *
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 *
 * @return exit status
 */
int main (int argc, char **argv)
{
	char *filename;
	
	printf("\n%s\n", header);
	if (argc != 2) {
		fprintf(stderr, "Usage: %s input\n", argv[0]);
		exit(-1);
	}

	filename = argv[1];
	run(filename);

	return 0;
}

