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
		[(c)2002,2003 Jorrit Tyberghein and others.],
		[http://cel.sourceforge.net])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])
AC_CANONICAL_HOST

#----------------------------------------------------------------------------
# Setup for the configuration header
#----------------------------------------------------------------------------

# nothing

#----------------------------------------------------------------------------
# Check for applications
#----------------------------------------------------------------------------
CS_PROG_CC
AS_IF([test -z "$CC"],
	[AC_MSG_ERROR([Couldn't find a usable C compiler.])])
CS_PROG_CXX
AS_IF([test -z "$CXX"],
        [AC_MSG_ERROR([Couldn't find a usable C++ compiler.])])
CS_PROG_LINK

AC_CHECK_PROGS([INSTALL], [install])
AS_IF([test -n "$INSTALL"], [CS_JAMCONFIG_PROPERTY([CMD.INSTALL], [$INSTALL])])

AC_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.14],
	[9.9|.9], [CS_JAMCONFIG_PROPERTY([CMD.SWIG], [$SWIG])])])

#----------------------------------------------------------------------------
# Determine system type
#----------------------------------------------------------------------------
CS_CHECK_HOST
                                                                                
#----------------------------------------------------------------------------
# Check for syntax problems / header files
#----------------------------------------------------------------------------
                                                                                
#nothing yet
                                                                                
#----------------------------------------------------------------------------
# Check for external libraries
#----------------------------------------------------------------------------

CS_PATH_CRYSTAL_JAM(0.97,,
  AC_MSG_ERROR([
*** Crystal Space couldn't be found. The latest version is always avaiable
*** from http://crystal.sourceforge.net
*** Be also sure that you've either installed CS or set the CRYSTAL var right.
]), csgeom csgfx csutil cstool cssys)
CS_PROG_SCFREG([CS_JAMCONFIG_PROPERTY([CMD.SCFREG],[$SCFREG])])

CS_CHECK_PYTHON
                                                                                
#----------------------------------------------------------------------------
# Package configuration switches
#----------------------------------------------------------------------------
AC_ARG_ENABLE([debug], [AC_HELP_STRING([--enable-debug],
      [build in debugging mode (default NO)])],
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
                                                                                
#----------------------------------------------------------------------------
# Output generated files.  Also create a Jamfile when compiling outside
# source directory.
#----------------------------------------------------------------------------
CS_JAMCONFIG_PROPERTY([JAMCONFIG_READ], [yes])
CS_JAMCONFIG_OUTPUT([Jamconfig])
CS_INIT_JAMFILE
AC_OUTPUT
                                                                                
AC_MSG_NOTICE([
                                                                                
Please note that this project uses jam as build tool.
So you have to type 'jam' now.
])

