#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autgen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize autoconf
#----------------------------------------------------------------------------
AC_INIT([cel], [0.97], [cel-main@lists.sourceforge.net])
CS_PACKAGEINFO([Crystal Entity Layer],
    [(C)2002-2004 Jorrit Tyberghein and others.],
    [http://cel.sourceforge.net/])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])
AC_CANONICAL_HOST

#----------------------------------------------------------------------------
# Check for applications
#----------------------------------------------------------------------------
CS_PROG_CC
AS_IF([test -z "$CC"],
    [AC_MSG_ERROR([Could not find a usable C compiler.])])
CS_PROG_CXX
AS_IF([test -z "$CXX"],
    [AC_MSG_ERROR([Could not find a usable C++ compiler.])])
CS_PROG_LINK

AC_CHECK_PROGS([INSTALL], [install])
CS_EMIT_BUILD_PROPERTY([INSTALL], [$INSTALL])

AC_PROG_RANLIB
CS_EMIT_BUILD_PROPERTY([RANLIB], [$RANLIB])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
CS_EMIT_BUILD_PROPERTY([CMD.DOXYGEN], [$DOXYGEN])

AC_CHECK_PROGS([MAKEINFO], [makeinfo])
CS_EMIT_BUILD_PROPERTY([CMD.MAKEINFO], [$MAKEINFO])

AC_CHECK_PROGS([PERL], [perl5 perl])
CS_EMIT_BUILD_PROPERTY([PERL], [$PERL])

AC_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.14],
	[9.9|.9], [CS_EMIT_BUILD_PROPERTY([CMD.SWIG], [$SWIG])])])

#----------------------------------------------------------------------------
# Determine system type
#----------------------------------------------------------------------------
CS_CHECK_HOST

#----------------------------------------------------------------------------
# Check for external libraries
#----------------------------------------------------------------------------
CS_PATH_CRYSTAL_JAM([0.97], [],
  [AC_MSG_ERROR([
*** Crystal Space could not be found. The latest version is always available
*** from http://crystal.sourceforge.net/
*** Also, be sure that you have either installed Crystal Space or set the
*** CRYSTAL environment variable properly.
])], [csgeom csgfx csutil cstool])

CS_CHECK_PYTHON

#----------------------------------------------------------------------------
# Package configuration switches
#----------------------------------------------------------------------------
AC_ARG_ENABLE([debug],
    [AC_HELP_STRING([--enable-debug], [build in debugging mode (default NO)])],
    [test "$enableval" = "yes" && cs_build_mode="debug"])
AC_MSG_CHECKING([build mode])
AS_VAR_SET_IF([cs_build_mode], [], [cs_build_mode=optimize])
AC_MSG_RESULT([$cs_build_mode])
CS_EMIT_BUILD_PROPERTY([MODE], [$cs_build_mode])

#------------------------------------------------------------------------------
# Determine whether the new renderer should be activated.
#------------------------------------------------------------------------------
AC_MSG_CHECKING([whether to use new renderer])
AC_ARG_ENABLE([new-renderer], [AC_HELP_STRING([--enable-new-renderer],
    [enable use of new renderer (_EXPERIMENTAL_!) (default NO)])],
    [], [enable_new_renderer=no])
AC_MSG_RESULT([$enable_new_renderer])
CS_EMIT_BUILD_PROPERTY([USE_NEW_RENDERER], [$enable_new_renderer])
AS_IF([test $enable_new_renderer = yes], 
  [CS_EMIT_BUILD_PROPERTY([SWIG.FLAGS], [-DCS_USE_NEW_RENDERER], [+])])

#-----------------------------------------------------------------------------
# Output Installpaths/package info
#-----------------------------------------------------------------------------
CS_OUTPUT_INSTALLDIRS
CS_EMIT_PACKAGEINFO([jam])

#----------------------------------------------------------------------------
# Output generated files.  Also create a Jamfile when compiling outside
# source directory.
#----------------------------------------------------------------------------
CS_EMIT_BUILD_PROPERTY([JAMCONFIG_READ], [yes])
CS_JAMCONFIG_OUTPUT([Jamconfig])
CS_INIT_JAMFILE
AC_OUTPUT

AC_MSG_NOTICE([

Please note that this project uses Jam as its build tool. This means
that you must type 'jam' to build it.

Jam home: http://www.perforce.com/jam/jam.html
Jam source: ftp://ftp.perforce.com/jam/
])
