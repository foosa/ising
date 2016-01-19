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
//#include "log.h"
#include "ising.h"


/**
 * @brief Parse the JSON root object for children
 *
 * @param root root object
 * @param output output object
 * @param field field key for the child
 * @param type JSON type
 */
json_t * parse_input_field (json_t *root, const char *field, json_type type)
{
	json_t * output;
	char *type_name = NULL;

	if (type == JSON_INTEGER) {
		type_name = "integer";
	}
	else if (type == JSON_REAL) {
		type_name = "real";
	}
	else if (type == JSON_ARRAY) {
		type_name = "array";
	}
	else if (type == JSON_OBJECT) {
		type_name = "object";
	}

	output = json_object_get(root, field);
	if (!(output)) {
		fprintf(stderr, "error: input missing required field %s\n", field);
		json_decref(root);
		exit(-1);
	}

	if (json_typeof(output) != type) {

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

	return output;
}


void run (const char *filename)
{
	json_t *root, *rows, *cols, *steps, *H, *J, *beta;
	json_error_t err;
	ising_t *im;
	size_t step, s;

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
	rows = parse_input_field(root, "rows", JSON_INTEGER);
	cols = parse_input_field(root, "cols", JSON_INTEGER);
	H = parse_input_field(root, "H", JSON_REAL);
	J = parse_input_field(root, "J", JSON_REAL);
	beta = parse_input_field(root, "beta", JSON_REAL);
	steps = parse_input_field(root, "steps", JSON_INTEGER);

	// Allocate the ising model
	im = ising_alloc(
			(size_t) json_integer_value(rows),
			(size_t) json_integer_value(cols),
			json_real_value(H),
			json_real_value(J),
			json_real_value(beta)
		);

	s = (size_t) json_integer_value(steps);
	for (step = 0; step < s; step++) {
		printf("%lu, %lu, % .10f, % i, % .10f\n", step, im->size,
			im->energy, im->mag, im->beta);
		ising_metropolis(im);
	}

	ising_free(im);
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

