# yoda.m4: Locate YODA headers and libraries for autoconf-based projects.
# author: James Robinson <james.robinson@cern.ch>
# date:   May 2013
AC_DEFUN([AC_CHECK_YODA],[
  AC_MSG_RESULT([checking for YODA])

  # Provide ability to set YODA location
  yodapath=""
  AC_ARG_WITH(yoda, AC_HELP_STRING(--with-yoda, prefix for YODA installation),
              [yodapath=${with_yoda}],
              [yodapath=${prefix}])

  # Add user path to search path for libraries and headers
  AS_IF([test "x${yodapath}" != "x${prefix}"], [
    LDFLAGS="${LDFLAGS} -L${yodapath}/lib"
    AC_SUBST(LDFLAGS)
    CPPFLAGS="$CPPFLAGS -I${yodapath}/include"
    AC_SUBST(CPPFLAGS)
  ])

  # Check for YODA library
  AC_CHECK_LIB(YODA, main, [], AC_MSG_ERROR([YODA library could not be located. Try using --with-yoda to specify its location]))

  # Check for YODA headers
  AC_CHECK_HEADERS([YODA/Histo1D.h], [], AC_MSG_ERROR([YODA headers could not be located. Try using --with-yoda to specify their location]))
])
