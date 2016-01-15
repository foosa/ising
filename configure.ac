# -*- Encoding: utf-8 -*-
#

AC_INIT(
	[ising],
	[0.1],
	[truemerrill@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])


AC_SEARCH_LIBS([json_object], [jansson], [],
	[AC_MSG_ERROR([The `jansson` library is not installed])])


AC_OUTPUT
