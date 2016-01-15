/*
 * -*- coding: utf-8 -*-
 *
 * Program:     log
 * Author:      True Merrill
 * Date:        Jan 05 2016
 *
 */


#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <jansson.h>
#include "log.h"


/* JSON formatting utilities */
json_t * json_safe_string (const char * string)
{
	if (string) {
		return json_string(string);
	}
	else {
		return json_null();
	}
}


json_t * json_log (const char *file, int line, log_level_t level,
	const char *format, va_list args)
{
	json_t *obj;
	time_t current;
	struct tm *p;
	char timestamp [32];
	char message [LOG_MAX_LENGTH];
	char labels [5][16] = {"DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"};
	char *label = labels[level];

	/* Get the timestamp */
	current = time(NULL);
	p = localtime(&current);
	strftime(timestamp, 32, "%c", p);

	/* Format the message */
	if (format) {
		vsnprintf(message, LOG_MAX_LENGTH, format, args);
	}
	else {
		snprintf(message, LOG_MAX_LENGTH, strerror(errno));
		errno = 0;
	}

	/* Create the json object */
	obj = json_object();
	json_object_set_new(obj, "level", json_safe_string(label));
	json_object_set_new(obj, "time", json_safe_string(timestamp));
	json_object_set_new(obj, "file", json_safe_string(file));
	json_object_set_new(obj, "line", json_integer(line));	
	json_object_set_new(obj, "message", json_safe_string(message));

	return obj;
}


void vlog_message (const char *file, int line, log_level_t level,
	const char *format, va_list args)
{
	json_t *obj;
	char *env_str;
	log_level_t env;

	/* Get the log level from the environment variable */
	env_str = getenv("LOG_LEVEL");
	if (env_str) {
		env = (log_level_t) atoi(env_str);
	}
	else {
		env = LOG_ERROR;
	}
	
	if (level >= env) {
		/* Create a JSON formatted log message */
		obj = json_log (file, line, level, format, args);
		json_dumpf(obj, stderr, JSON_PRESERVE_ORDER);
		fprintf(stderr, "\n");
		json_decref(obj);
	}
}


void log_message (const char *file, int line, log_level_t level,
	const char *format, ...)
{
	va_list args;
	va_start (args, format);
	vlog_message (file, line, level, format, args);
	va_end (args);
}


