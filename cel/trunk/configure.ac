#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autgen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize autoconf
#----------------------------------------------------------------------------
AC_INIT([cel], [0.99], [cel-main@lists.sourceforge.net])
CS_PACKAGEINFO([Crystal Entity Layer],
    [Copyright (C)2002-2004 Jorrit Tyberghein and others.],
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

# The default RANLIB in Jambase is wrong on some platforms, and is also
# unsuitable during cross-compilation, so we set the value unconditionally
# (sixth argument of CS_EMIT_BUILD_PROPERTY).
AC_PROG_RANLIB
CS_EMIT_BUILD_PROPERTY([RANLIB], [$RANLIB], [], [], [], [Y])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
CS_EMIT_BUILD_PROPERTY([CMD.DOXYGEN], [$DOXYGEN])

AC_CHECK_PROGS([MAKEINFO], [makeinfo])
CS_EMIT_BUILD_PROPERTY([CMD.MAKEINFO], [$MAKEINFO])

AC_CHECK_PROGS([PERL], [perl5 perl])
CS_EMIT_BUILD_PROPERTY([PERL], [$PERL])

AC_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.20],
	[9.9|.9], [CS_EMIT_BUILD_PROPERTY([CMD.SWIG], [$SWIG])])])

CS_CHECK_MKDIR
CS_EMIT_BUILD_PROPERTY([CMD.MKDIR], [$MKDIR])
CS_EMIT_BUILD_PROPERTY([CMD.MKDIRS], [$MKDIRS])

#----------------------------------------------------------------------------
# Determine system type
#----------------------------------------------------------------------------
CS_CHECK_HOST

#----------------------------------------------------------------------------
# Check for external libraries
#----------------------------------------------------------------------------
AC_ARG_WITH([cs], [AC_HELP_STRING([--without-cs],
  [do not look for an installed Crystal Space SDK; this option is useful only
  if you need to perform routine maintenance tasks, such as generating
  project files or converting documentation, without having the Crystal Space
  SDK installed])])
AS_IF([test -z "$with_cs"], [with_cs=yes])

AS_IF([test $with_cs != no],
[CS_PATH_CRYSTAL_JAM([0.99], [],
  [AC_MSG_ERROR([
*** Crystal Space could not be found. The latest version is always available
*** from http://www.crystalspace3d.org/
*** Also, be sure that you have either installed Crystal Space or set the
*** CRYSTAL environment variable properly.
])], [csgeom csgfx csutil cstool])])

CS_EMIT_CHECK_PYTHON

#------------------------------------------------------------------------------
# Check if TemplateToolkit2 is installed (needed for MSVC project generation).
#------------------------------------------------------------------------------
AS_IF([test -n "$PERL"],
    [AC_CACHE_CHECK([for TemplateToolkit], [cs_cv_perl_tt2],
	[AS_IF([AC_RUN_LOG(
	    [$PERL -M'Template 2.11' -MTemplate::Plugin -e 0 1>&2])],
	    [cs_cv_perl_tt2=yes],
	    [cs_cv_perl_tt2=no])])
    AC_PATH_PROGS([TTREE], [ttree])
    AS_IF([test $cs_cv_perl_tt2 = yes && test -n "$TTREE"],
	[CS_EMIT_BUILD_PROPERTY([TTREE], [$TTREE])])])

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

#-----------------------------------------------------------------------------
# Output Installpaths/package info
#-----------------------------------------------------------------------------
CS_OUTPUT_INSTALLDIRS
CS_EMIT_PACKAGEINFO

#----------------------------------------------------------------------------
# Output generated files.  Also create a Jamfile when compiling outside
# source directory.
#----------------------------------------------------------------------------
CS_JAMCONFIG_OUTPUT([Jamconfig])
AC_CONFIG_FILES([Jamfile])
AC_OUTPUT

AC_MSG_NOTICE([

Please note that this project uses Jam as its build tool,
therefore you must type 'jam' to build the project.

Jam home: http://www.perforce.com/jam/jam.html
Jam source: ftp://ftp.perforce.com/jam/
])
