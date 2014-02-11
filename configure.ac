AC_INIT([odep], [0.0.1])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

CFLAGS=" $CFLAGS"
CXXFLAGS=" $CXXFLAGS"
LDFLAGS=" $LDFLAGS"

AC_ARG_ENABLE(debug,   [  --enable-debug         enable debug support],,enable_debug=no)

if test "$enable_debug" = yes ; then
	CFLAGS=" $CFLAGS -DDEBUG"
	CXXFLAGS=" $CXXFLAGS -DDEBUG"
	AC_DEFINE(DEBUG, 1, [Define if debugging is enabled.])
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([gethostbyname gettimeofday memmove memset pow select socket])

AC_CONFIG_SUBDIRS(libcommon)
AC_OUTPUT( Makefile src/Makefile Doxyfile)

echo
echo $PACKAGE $VERSION configured
echo
