#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([vinterm], [0.1.0], [http://code.google.com/p/vinterm/issues/])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/main.cc])

# Checks for programs, clear CFLAGS
OLD_CXXFLAGS=$CXXFLAGS
AC_PROG_CXX
CXXFLAGS=$OLD_CXXFLAGS

# debug compilation support
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])
if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CPPFLAGS="$AM_CPPFLAGS -g -Wall -Wno-uninitialized -O0"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CPPFLAGS="$AM_CPPFLAGS -O3"
fi
AC_SUBST([AM_CPPFLAGS])

# DUMA support
AC_MSG_CHECKING([whether to compile with DUMA support])
AC_ARG_ENABLE([duma],
    [AS_HELP_STRING([--enable-duma],
        [enable DUMA support (def=no)])],
    [dumait="$enableval"],
    [dumait=no])
AC_MSG_RESULT([$dumait])
if test x"$dumait" = x"yes"; then
	AC_CHECK_LIB(duma, malloc, [], [
		echo "Error! You need to have the libduma library."
		exit -1
		])
fi

# Checks for SDL libraries
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_CHECK_LIB(stdc++, main)

#AC_CHECK_LIB(config, config_read, [], [
#	echo "Error! You need to have the libconfig++ library."
#	exit -1
#	])

# Checks for header files.
AC_CONFIG_HEADERS([config.h])

# Checks for typedefs, structures, and compiler characteristics.

# Makefiles
AC_CONFIG_FILES([Makefile src/Makefile data/Makefile])

# Checks for library functions.
AC_OUTPUT
