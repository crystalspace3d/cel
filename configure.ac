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

#------------------------------------------------------------------------------
# CS_EMIT_BUILD_FLAGS(MESSAGE, CACHE-VAR, FLAGS, [LANGUAGE],
#                     MAKEFILE-VARIABLE, [APPEND], [CACHE-VALUE],
#                     [ACTION-IF-RECOGNIED], [ACTION-IF-NOT-RECOGNIED])
#       Check if the compiler or linker recognizes a command-line option.
#       MESSAGE is the "checking" message.  CACHE-VAR is the shell cache
#       variable which receives the flag recognized by the compiler or linker,
#       or "no" if the flag was not recognized.  FLAGS is a
#       whitespace-delimited list of build tuples created with
#       CS_CREATE_TUPLE().  Each tuple from FLAGS is attempted in order until
#       one is found which is recognized by the compiler.  After that, no
#       further flags are checked.  LANGUAGE is typically either C or C++ and
#       specifies which compiler to use for the test.  If LANGUAGE is omitted,
#       C is used.  MAKEFILE-VARIABLE is the name of the makefile variable to
#       insert into the makefile and Jam text caches if a usable flag is
#       encountered.  CACHE-VALUE is the value to assign to the makefile
#       variable.  If omitted, it defaults to $cs_build_cflags.  If you are
#       checking linker flags, rather than compiler flags, then you should
#       specify $cs_build_lflags for this argument.  If APPEND is not the empty
#       string, then CACHE-VALUE is appended to the existing value of the
#       makefile variable.  If the command-line option was recognized, then
#       ACTION-IF-RECOGNIZED is invoked, otherwise ACTION-IF-NOT-RECOGNIZED is
#       invoked.
#------------------------------------------------------------------------------
AC_DEFUN([CS_EMIT_BUILD_FLAGS],
    [AC_CACHE_CHECK([$1], [$2],
        [CS_BUILD_IFELSE([], [$3], [$4],
            [$2=m4_default([$7],[$cs_build_cflags])], [$2=no])])
    AS_IF([test "$$2" != no],
        [$8
        CS_JAMCONFIG_PROPERTY([$5], [$$2], [$6])],
        [m4_default([$9],[:])])])

#------------------------------------------------------------------------------
# Check if Mingw compiler recognizes -fvtable-thunks.  As of gcc 3.x, this
# option is no longer supported.  Unfortunately, rather than returning an error
# code (as it does with other unrecognized options), the compiler merely prints
# a warning message when -fvtable-thunks is used, thus we use -Werror to turn
# the warning into an error.
#------------------------------------------------------------------------------
AC_DEFUN([CS_CHECK_MINGW_VTABLE_THUNKS],
    [AC_REQUIRE([AC_CANONICAL_HOST])
    case $host_os in
        mingw*)
            CS_EMIT_BUILD_FLAGS([if -fvtable-thunks is needed],
                [cs_cv_prog_cxx_vtable_thunks],
                [CS_CREATE_TUPLE([-fvtable-thunks -Werror])],
                [C++], [COMPILER.C++FLAGS], [+=], [-fvtable-thunks])
            ;;
    esac])
                                                                                
CS_CHECK_MINGW_VTABLE_THUNKS
                               
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
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
                                                                                
AC_MSG_NOTICE([
Please note that this project uses Jam as its preferred build tool,
so you can type 'jam' to build it.  (Building the project with GNU
'make' is also supported, though deprecated.)
])
