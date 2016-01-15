# -*- Encoding: utf-8 -*-
#

AC_INIT(
	[ising],
	[0.0],
	[truemerrill@gmail.com]
)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_CC


AC_CONFIG_FILES([
	Makefile
	src/Makefile
])


AC_SEARCH_LIBS([json_object], [jansson], [],
	[AC_MSG_ERROR([The `jansson` library is not installed])])


AC_OUTPUT
