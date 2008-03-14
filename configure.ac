## Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Yoda],[0.1.0],[yoda@projects.hepforge.org],[Yoda])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/Bin.cc])
AC_CONFIG_HEADERS([include/YODA/YodaConfig.hh])

## OS X
AC_CEDAR_OSX

## Set default compiler flags
if test "x$CXXFLAGS" == "x"; then CXXFLAGS="-O3"; fi

## Make compiler error messages more readable if gfilt is installed.
#AC_CEDAR_CXXFILTER

## Checks for programs.
AC_LANG(C++)
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_DISABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

## Work out library suffix for the build
LIB_SUFFIX=\\\"$shrext_cmds\\\"
AC_SUBST([LIB_SUFFIX])

## Build Doxygen if possible
AC_PATH_PROG(DOXYGEN, doxygen)
AM_CONDITIONAL(WITH_DOXYGEN, test "$DOXYGEN")

## Decide whether to build the Python extension
AC_ARG_ENABLE(pyext, [AC_HELP_STRING(--enable-pyext, [build Yoda Python module @<:@default=yes@:>@])], [], [enable_pyrivet=yes])
AM_CONDITIONAL(ENABLE_PYEXT, [test x$enable_pyext == xyes])
if test x$enable_pyext == xyes; then
  AM_PATH_PYTHON(2.3)
  AC_PROG_SWIG(1.3.21)
  SWIG_ENABLE_CXX
  SWIG_PYTHON
fi

## Optional ROOT analysis framework
#AC_ARG_ENABLE([ROOT], [AC_HELP_STRING(--enable-root, [build with ROOT output @<:@default=yes@:>@])], [], [enable_root=yes])
#if test "x$enable_root" != "xno"; then
#  AC_SEARCH_ROOT([with_root=yes
#    AM_CXXFLAGS="$AM_CXXFLAGS -Wno-long-long"
#    AC_TEST_ROOT], [AC_MSG_WARN([ROOT functionalities not available])])
#  AM_CONDITIONAL(WITH_ROOT, [test x$with_root == xyes])
#fi

## Set default build flags
AM_CPPFLAGS="-I\$(top_srcdir)/include -I\$(top_builddir)/include"
AM_CXXFLAGS="$AM_CXXFLAGS -ansi -pedantic -Wall -DLIB_SUFFIX=$LIB_SUFFIX "

## Debug flag (default=none)
AC_ARG_ENABLE([debug], [AC_HELP_STRING(--enable-debug, [build with debugging symbols  @<:@default=no@:>@])], [], [enable_debug=no])
if test x$enable_debug == xyes; then
  [AM_CXXFLAGS="$AM_CXXFLAGS -g"]
fi

AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)

AC_EMPTY_SUBST
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile include/YODA/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([tests/Makefile])
#AC_CONFIG_FILES([pyext/Makefile])
#AC_CONFIG_FILES([bin/Makefile bin/yoda-config])

AC_OUTPUT
