#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autogen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize autoconf.
#----------------------------------------------------------------------------
AC_INIT([cel], [0.99], [cel-main@lists.sourceforge.net])
CS_PACKAGEINFO([Crystal Entity Layer],
    [Copyright (C)2002-2005 Jorrit Tyberghein and others.],
    [http://cel.crystalspace3d.org/])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])
AC_CANONICAL_HOST

#----------------------------------------------------------------------------
# Check for tools.
#----------------------------------------------------------------------------
CS_PROG_CC
CS_PROG_CXX
CS_PROG_LINK

CS_CHECK_COMMON_TOOLS_LINK
CS_CHECK_COMMON_TOOLS_BASIC
CS_CHECK_COMMON_TOOLS_DOC_TEXINFO
CS_CHECK_COMMON_TOOLS_DOC_DOXYGEN

CS_CHECK_PROGS([PERL], [perl5 perl])
CS_EMIT_BUILD_PROPERTY([PERL], [$PERL])

CS_CHECK_TEMPLATE_TOOLKIT2([emit])

CS_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.22],
	[9.9|.9], [CS_EMIT_BUILD_PROPERTY([CMD.SWIG], [$SWIG])])])

#----------------------------------------------------------------------------
# Determine system type.
#----------------------------------------------------------------------------
CS_CHECK_HOST

#------------------------------------------------------------------------------
# Check how to enable and disable compilation warnings, and how to promote
# diagnostics from warning to error status.
#------------------------------------------------------------------------------
CS_COMPILER_WARNINGS([C++], [cs_cv_prog_cxx_enable_warnings],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS],
	[$cs_cv_prog_cxx_enable_warnings], [+])])

CS_COMPILER_IGNORE_UNUSED([C++], [cs_cv_prog_cxx_ignore_unused],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS.WARNING.NO_UNUSED],
	[$cs_cv_prog_cxx_ignore_unused])])

CS_COMPILER_IGNORE_UNINITIALIZED([C++], [cs_cv_prog_cxx_ignore_uninitialized],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS.WARNING.NO_UNINITIALIZED],
	[$cs_cv_prog_cxx_ignore_uninitialized])])

#------------------------------------------------------------------------------
# Check for compiler debug flags.
#------------------------------------------------------------------------------
binutils_recent=no
AS_IF([test -n "$OBJCOPY"],
    [CS_CHECK_PROG_VERSION([binutils], [$OBJCOPY -V], [2.16], [9.9|.9|.9|.9],
        [binutils_recent=yes])])

AC_MSG_CHECKING([whether to split debug information])
AC_ARG_ENABLE([separate-debug-info],
    [AC_HELP_STRING([--enable-separate-debug-info],
        [split debug information into separate files. (default YES if
        binutils >= 2.16 detected and not on Cygwin, otherwise default NO)])])
AS_IF([test -z "$enable_separate_debug_info"],
    AS_IF([test -z `uname | grep -i CYGWIN`],
        [enable_separate_debug_info=$binutils_recent],
        [enable_separate_debug_info=no]))
AC_MSG_RESULT([$enable_separate_debug_info])

CS_EMIT_BUILD_PROPERTY([LINK.DEBUG.INFO.SEPARATE], [$enable_separate_debug_info])

CS_EMIT_BUILD_FLAGS([how to enable debug mode debugging symbols],
    [cs_cv_prog_cxx_debug_symbols_extra],
    [CS_CREATE_TUPLE([-g3]) CS_CREATE_TUPLE([-g2]) CS_CREATE_TUPLE([-g])],
    [C++], [COMPILER.CFLAGS.debug], [+],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.LFLAGS.debug],
        [$cs_cv_prog_cxx_debug_symbols_extra], [+])])

AC_MSG_CHECKING([whether to enable debug information in optimize mode])
AC_ARG_ENABLE([optimize-mode-debug-info],
    [AC_HELP_STRING([--enable-optimize-mode-debug-info],
        [whether to enable debug information even in optimize builds.
        (default YES if separate debug information is enabled,
        otherwise default NO)])],
    [enable_optimize_mode_debug_info=$enableval])
AS_IF([test -z "$enable_optimize_mode_debug_info"],
    [enable_optimize_mode_debug_info=$enable_separate_debug_info])
AC_MSG_RESULT([$enable_optimize_mode_debug_info])
AS_IF([test "$enable_optimize_mode_debug_info" != "no"],
    [CS_EMIT_BUILD_FLAGS([how to enable optimize mode debugging symbols],
        [cs_cv_prog_cxx_debug_symbols_normal],
        [CS_CREATE_TUPLE([-g2]) CS_CREATE_TUPLE([-g])],
        [C++], [COMPILER.CFLAGS.optimize], [+],
        [CS_EMIT_BUILD_PROPERTY([COMPILER.LFLAGS.optimize],
            [$cs_cv_prog_cxx_debug_symbols_normal], [+])])])

#------------------------------------------------------------------------------
# Check how to set default symbol visibility, and how to override the setting
# in code.
#------------------------------------------------------------------------------
CS_CHECK_BUILD_FLAGS([for symbol visibility flag],
    [cs_cv_prog_cc_visibility_hidden],
    [CS_CREATE_TUPLE([-fvisibility=hidden])])
AS_IF([test -n "$cs_cv_prog_cc_visibility_hidden"],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS], 
	[$cs_cv_prog_cc_visibility_hidden], [+])])

#------------------------------------------------------------------------------
# Check if "hidden" visibilty for inline functions is supported.
#
# IMPLEMENTATION NOTES
#
# There is an obscure bug in gcc 3.4.x and 4.0.x where
# -fvisibility-inlines-hidden on AMD64 in combination with -fPIC and -shared
# flags causes the linker to fail with a bogus error stating that the target
# library needs to be built with -fPIC even when it has been built using that
# option. On such installations, if Crystal Space is built with
# -fvisibility-inlines-hidden, then clients liking against Crystal Space
# experience this bogus error.  Normally, we would test for this anomaly by
# manipulating a std::basic_string<> in a test program, and avoid
# -fvisibility-inlines-hidden if the test program fails to link, however,
# practical experience has shown that this test is not 100% reliable for all
# such installations. Instead, for simplicity, we disable this flag presently
# for all versions of gcc on AMD64.  References:
#
#     http://dev.gentoo.org/~kugelfang/pappy-visibility.txt
#     http://www.gnu.org/software/gcc/gcc-4.0/changes.html#visibility
#     http://www.nedprod.com/programs/gccvisibility.html
#     http://bugs.gentoo.org/show_bug.cgi?id=78720
#
# Furthermore, there seems to be a bug on MacOS/X with gcc 4.0.x where use of
# -fvisibility-inlines-hidden causes link problems with external clients which
# do not specify this flag.  For this reason, we also disable this flag on
# MacOS/X.  In particular, clients receive errors of this sort:
#
#    /usr/bin/ld: libcrystalspace.a(scf.o) malformed object, illegal
#        reference for -dynamic code (reference to a coalesced section
#        (__TEXT,__textcoal_nt) from section (__TEXT,__text) relocation
#        entry (1))
#------------------------------------------------------------------------------
CS_CHECK_BUILD_FLAGS([for inline visibility flag],
    [cs_cv_prog_cxx_visibility_inlines_hidden],
    [CS_CREATE_TUPLE([-fvisibility-inlines-hidden])],[C++])

AS_IF([test -n "$cs_cv_prog_cxx_visibility_inlines_hidden"],
    [AC_CACHE_CHECK([if $cs_cv_prog_cxx_visibility_inlines_hidden is buggy],
	[cs_cv_prog_cxx_visibility_inlines_hidden_buggy],
	[AS_IF([test $ac_compiler_gnu = yes],
	    [AS_IF([test $host_cpu = x86_64 || test x$cs_host_macosx = xyes],
		[cs_cv_prog_cxx_visibility_inlines_hidden_buggy=yes],
		[cs_cv_prog_cxx_visibility_inlines_hidden_buggy=no])],
	    [cs_cv_prog_cxx_visibility_inlines_hidden_buggy=no])])],
    [cs_cv_prog_cxx_visibility_inlines_hidden_buggy=no])

AS_IF([test $cs_cv_prog_cxx_visibility_inlines_hidden_buggy != yes],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS],
	[$cs_cv_prog_cxx_visibility_inlines_hidden], [+])])



#----------------------------------------------------------------------------
# Check for external libraries.
#----------------------------------------------------------------------------
CS_CHECK_CSWIN32LIBS
CS_CHECK_PYTHON()
CS_CHECK_CPPUNIT([emit])
CS_CHECK_PTHREAD([cygwin*])

CS_CHECK_LIB_WITH([NL],
    [AC_LANG_PROGRAM([[#include <nl.h>]], [nlOpen(0,0)])],
    [], [C++],
    [CS_EMIT_BUILD_RESULT([cs_cv_libNL], [HAWKNL])], [],
    [$cs_cv_sys_pthread_cflags],
    [$cs_cv_sys_pthread_lflags],
    [$cs_cv_sys_pthread_libs])

#------------------------------------------------------------------------------
# Use the CPPFLAGS, CXXFLAGS, CFLAGS, and LDFLAGS passed to configure, as well
# as any additional flags provided by cs-win32libs.
#------------------------------------------------------------------------------
CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS], [$CPPFLAGS $CFLAGS], [+])
CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS], [$CPPFLAGS $CXXFLAGS], [+])
CS_EMIT_BUILD_PROPERTY([COMPILER.LFLAGS], [$LDFLAGS], [+])

#----------------------------------------------------------------------------
# Check for Crystal Space (CS)
#----------------------------------------------------------------------------
AC_ARG_WITH([cs], [AC_HELP_STRING([--without-cs],
  [do not look for an installed Crystal Space SDK; this option is useful only
  if you need to perform routine maintenance tasks, such as generating
  project files or converting documentation, without having the Crystal Space
  SDK installed])])
AS_IF([test -z "$with_cs"], [with_cs=yes])

AS_IF([test $with_cs != no],
    [AS_IF([test $cs_cv_python != no],
	[cs_opt_libs="crystalspace_python"], [cs_opt_libs=''])
    CS_PATH_CRYSTAL_EMIT([0.99], [],
	[AC_MSG_ERROR([
*** Crystal Space could not be found or was unusable. The latest version is
*** always available from http://www.crystalspace3d.org/
*** Also, be sure that you have either installed Crystal Space or set the
*** CRYSTAL environment variable properly.
])], [crystalspace], [$cs_opt_libs])])

#----------------------------------------------------------------------------
# We checked for Python earlier, and used that result to adjust the CS test
# above, but whether we actually build CEL's Python module depends upon whether
# CS was built with Python support, which we can only know at this point (after
# running the CS test).
#----------------------------------------------------------------------------
AS_IF([test $with_cs != no && test $cs_cv_python != no],
    [AC_MSG_CHECKING([for Crystal Space Python support])
    cel_do_python=no
    for m in $CRYSTAL_AVAILABLE_LIBS; do
	AS_IF([test crystalspace_python = "$m"],
	    [cel_do_python=yes
	    break])
    done
    AC_MSG_RESULT([$cel_do_python])
    AS_IF([test $cel_do_python = yes],
	[CS_EMIT_BUILD_RESULT([cs_cv_python], [PYTHON])])])

#----------------------------------------------------------------------------
# Check if the Crystal Space static plugins lib was built.
#----------------------------------------------------------------------------
AS_IF([test $with_cs != no],
    [AC_MSG_CHECKING([for Crystal Space static plugins])
    cel_have_crystal_static=no
    for m in $CRYSTAL_AVAILABLE_LIBS; do
	AS_IF([test crystalspace_staticplugins = "$m"],
	    [cel_have_crystal_static=yes
	    break])
    done
    AC_MSG_RESULT([$cel_have_crystal_static])
    CS_EMIT_BUILD_PROPERTY([HAVE_STATIC_PLUGINS], [$cel_have_crystal_static])])

#------------------------------------------------------------------------------
# Determine if plugin meta-information should be embedded directly into plugin
# modules or if it should exist in stand-alone .csplugin files.
#------------------------------------------------------------------------------
CS_META_INFO_EMBED([emit], [no])
AS_IF([test $enable_meta_info_embedding = yes],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS],
	[AS_ESCAPE([$(EMBED_META.CFLAGS)])], [+])
    CS_EMIT_BUILD_PROPERTY([COMPILER.LFLAGS],
	[AS_ESCAPE([$(EMBED_META.LFLAGS)])], [+])])

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
AC_CONFIG_FILES([Jamfile scripts/autopackage/celstart_static.apspec])
AC_OUTPUT

AC_MSG_NOTICE([

Please note that this project uses Jam as its build tool,
therefore you must type 'jam' to build the project.

Jam home: http://www.perforce.com/jam/jam.html
Jam source: ftp://ftp.perforce.com/jam/
])
