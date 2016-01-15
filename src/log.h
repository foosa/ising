/*
 * -*- coding: utf-8 -*-
 *
 * Program:     log
 * Author:      True Merrill
 * Date:        Jan 05 2016
 *
 */


#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

#define LOG_MAX_LENGTH 256


typedef enum
{
	LOG_DEBUG, 
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_CRITICAL
} log_level_t;


/**
* @brief Log a message to output.
*
* @param file filename of the source code that threw the log message
* @param line line number of the source code that threw the log message
* @param level logging level
* @param format message
* @param args varadic arguments
*/
void vlog_message (const char *file, int line, log_level_t level, const char *format, va_list args);
void log_message (const char *file, int line, log_level_t level, const char * format, ...);

#define debug(M, ...) log_message (__FILE__, __LINE__, LOG_DEBUG, M, ##__VA_ARGS__)
#define info(M, ...) log_message (__FILE__, __LINE__, LOG_INFO, M, ##__VA_ARGS__)
#define warn(M, ...) log_message (__FILE__, __LINE__, LOG_WARN, M, ##__VA_ARGS__)
#define error(M, ...) log_message (__FILE__, __LINE__, LOG_ERROR, M, ##__VA_ARGS__)
#define critical(M, ...) log_message (__FILE__, __LINE__, LOG_CRITICAL, M, ##__VA_ARGS__)


#endif /* end of include guard: LOG_H */
