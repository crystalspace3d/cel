#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autogen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize autoconf.
#----------------------------------------------------------------------------
# Don't forget to update include/celversion.h, docs/texinfo/version.txi
# and mk/autoconf/cel.m4 when changing this version number.
AC_INIT([cel], [1.4], [cel-main@lists.sourceforge.net])
CS_PACKAGEINFO([Crystal Entity Layer],
    [Copyright (C)2002-2008 Jorrit Tyberghein and others.],
    [http://www.crystalspace3d.org/main/CEL])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])
AC_CANONICAL_HOST
CS_PATH_INIT

#----------------------------------------------------------------------------
# Check for tools.
#----------------------------------------------------------------------------
CS_CHECK_MNO_CYGWIN

CS_PROG_CC
CS_PROG_CXX
CS_PROG_LINK

CS_CHECK_COMMON_TOOLS_LINK
CS_CHECK_COMMON_TOOLS_BASIC
CS_CHECK_COMMON_TOOLS_DOC_TEXINFO
CS_CHECK_COMMON_TOOLS_DOC_DOXYGEN
CS_CHECK_COMMON_TOOLS_ICONS

CS_CHECK_PROGS([PERL], [perl5 perl])
CS_EMIT_BUILD_PROPERTY([PERL], [$PERL])

CS_CHECK_TEMPLATE_TOOLKIT2([emit])

CS_CHECK_PROGS([SWIG], [swig])
AS_IF([test -n "$SWIG"],
    [CS_CHECK_PROG_VERSION([swig], [$SWIG -version 2>&1], [1.3.22],
	[9.9|.9],
        [CS_EMIT_BUILD_PROPERTY([CMD.SWIG], [$SWIG])
        CS_EMIT_BUILD_PROPERTY([CMD.SWIG.VERSION], [$cs_prog_swig_version])])])


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
# Check for option to disable strict-aliasing (enabled by gcc on higher 
# optimization levels). This is needed for some code we don't have
# control over (e.g. SWIG-generated code) that breaks strict-aliasing rules.
#------------------------------------------------------------------------------
CS_CHECK_BUILD_FLAGS([for flag to disable string-aliasing],
    [cs_cv_prog_cxx_no_strict_aliasing],
    [CS_CREATE_TUPLE([-fno-strict-aliasing])], [C++])

AS_IF([test $cs_cv_prog_cxx_no_strict_aliasing != no],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS.STRICTALIASING.DISABLE],
	[$cs_cv_prog_cxx_no_strict_aliasing], [+])])


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
# Check how to declare symbol visibility.
#------------------------------------------------------------------------------
CS_VISIBILITY_FLAG_INLINES_HIDDEN(
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS],
	[$cs_prog_cxx_visibility_inlines_hidden], [+])])

CS_VISIBILITY_FLAG_HIDDEN([C], [],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS], 
	[$cs_cv_prog_c_visibility_hidden], [+])])


#----------------------------------------------------------------------------
# Check for external libraries.
#----------------------------------------------------------------------------
CS_CHECK_CSWIN32LIBS
CS_CHECK_PYTHON([emit])
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
    [CS_PATH_CRYSTAL_EMIT([1.1], [],
	[AC_MSG_ERROR([
*** Crystal Space could not be found or was unusable. The latest version is
*** always available from http://www.crystalspace3d.org/
*** Also, be sure that you have either installed Crystal Space or set the
*** CRYSTAL environment variable properly.
])], [crystalspace], [$cs_opt_libs])])


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
# Check for relaytool. (After CS check so we can use the version
# shipped with CS.)
#------------------------------------------------------------------------------
CS_CHECK_COMMON_TOOLS_RELAYTOOL([$CRYSTAL_EXEC_PREFIX/bin])



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


#------------------------------------------------------------------------------
# Determine whether link libraries should be static or shared.
#------------------------------------------------------------------------------
AC_MSG_CHECKING([whether to build shared libraries])
case $host_os in
    cygwin*|linux*) enable_shared_default=yes ;;
    *) enable_shared_default=no ;;
esac
AC_ARG_ENABLE([shared], [AC_HELP_STRING([--enable-shared],
    [enable creation of shared libraries (default YES on Win32 and Linux, 
    NO otherwise)])],
    [], [enable_shared=$enable_shared_default])
AC_MSG_RESULT([$enable_shared])
CS_EMIT_BUILD_PROPERTY([BUILD_SHARED_LIBS], [$enable_shared])
AS_IF([test $enable_shared = yes], 
  [CS_HEADER_PROPERTY([CEL_BUILD_SHARED_LIBS])])


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
CS_HEADER_OUTPUT([include/celconfig.h])
CS_JAMCONFIG_OUTPUT([Jamconfig])
AC_CONFIG_FILES([Jamfile scripts/autopackage/celstart_static.apspec])
AC_OUTPUT

AC_MSG_NOTICE([

Please note that this project uses Jam as its build tool,
therefore you must type 'jam' to build the project.

Jam home: http://www.perforce.com/jam/jam.html
Jam source: ftp://ftp.perforce.com/jam/
])
