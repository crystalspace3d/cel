#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autgen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize autoconf
#----------------------------------------------------------------------------
AC_INIT([cel], [0.97], [cel-main@lists.sourceforge.net])
CS_PACKAGEINFO([Crystalspace Entity Layer],
    [(C)2002,2003 Jorrit Tyberghein and others.],
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

AC_CHECK_PROGS([DOXYGEN], [doxygen])
AS_IF([test -n "$DOXYGEN"], [CS_JAMCONFIG_PROPERTY([CMD.DOXYGEN], [$DOXYGEN])])

AC_CHECK_PROGS([INSTALL], [install])
AS_IF([test -n "$INSTALL"], [CS_JAMCONFIG_PROPERTY([INSTALL], [$INSTALL])])

# Jambase in Jam 2.4 has a bug where it incorrectly defines RANLIB as "" for
# MacOS/X, and this bogus value will override a RANLIB set via ?= in Jamconfig,
# thus we must use CS_JAMCONFIG_PROPERTY()'s "unconditional" option to force a
# correct value for RANLIB.  This is especially important for MacOS/X Panther
# which expects us to use the -s option with ranlib, which we can accomplish
# via "RANLIB += -s"; but this will work correctly only if RANLIB has a
# meaningful value in the first place.
AC_PROG_RANLIB
AS_IF([test -n "$RANLIB"],
    [CS_JAMCONFIG_PROPERTY([RANLIB], [$RANLIB], [], [Y])])

AC_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.14],
	[9.9|.9], [CS_JAMCONFIG_PROPERTY([CMD.SWIG], [$SWIG])])])

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
CS_JAMCONFIG_PROPERTY([MODE], [$cs_build_mode])

#-----------------------------------------------------------------------------
# Output Installpaths/package info
#-----------------------------------------------------------------------------
CS_OUTPUT_INSTALLDIRS
CS_EMIT_PACKAGEINFO([jam])

# For Makefile.in
AC_SUBST([INSTALLDIR_PLUGIN], [$libdir/$PACKAGE_NAME])

#----------------------------------------------------------------------------
# Output generated files.  Also create a Jamfile when compiling outside
# source directory.
#----------------------------------------------------------------------------
CS_JAMCONFIG_PROPERTY([JAMCONFIG_READ], [yes])
CS_JAMCONFIG_OUTPUT([Jamconfig])
CS_INIT_JAMFILE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
                                                                                
AC_MSG_NOTICE([
Please note that this project uses Jam as its preferred build tool,
so you can type 'jam' to build it.  (Building the project with GNU
'make' is also supported, though deprecated and not necessarily well
maintained.)
])
