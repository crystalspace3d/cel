#-----------------------------------------------------------------------------
# Python detection macros
# original macros by Eric Sunshine
# extracted and modified by Matze Braun <matze@braunis.de>
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Check if Python SDK is installed and which compiler and linker flags are
# required to embed Python.  Several techniques are used in an attempt to
# determine the proper flags.  First, an attempt it made to query Python's
# "distutils.sysconfig" module.  This should work for Python 2.x.  If that
# fails, Python's "sys" module is queried to find the path of the Python
# standard library.  Inside the library's "config" directory is a makefile
# which can be parsed to glean the necessary flags.  If the Python executable
# is not available, try locating the Python directories and Makefile manually.
# As a last resort, if the environment variables PYTHON_INC and PYTHON_LIB are
# set, then use those values.
#-----------------------------------------------------------------------------
dnl CS_CHECK_PYTHON([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN([CS_CHECK_PYTHON],
[

AC_CHECK_PROGS([PYTHON], [python])
AS_IF([test -n "$PYTHON"], [CS_JAMCONFIG_PROPERTY([PYTHON], [$PYTHON])])

AS_IF([test -n "$PYTHON"],
    [AC_CACHE_CHECK([for python distutils module], [cs_cv_python_distutils],
	[AS_IF([AC_RUN_LOG([$PYTHON -c 'import distutils.sysconfig' 1>&2])],
	    [cs_cv_python_distutils=yes], [cs_cv_python_distutils=no])])
    AS_IF([test $cs_cv_python_distutils = yes],
	[CS_JAMCONFIG_PROPERTY([PYTHON.DISTUTILS], [yes])])],
    [cs_cv_python_distutils=no])

AC_CACHE_CHECK([for python SDK], [cs_cv_python_sdk],
    [cs_cv_python_sdk=no
    cs_python_lib_dir=''
    cs_python_makefile=''

    AS_IF([test -n "$PYTHON"],
	[# Python 2.x?
	AS_IF([test $cs_cv_python_distutils = yes],
	    [cs_cv_python_base=`AC_RUN_LOG([$PYTHON -c \
		'import distutils.sysconfig ; import os.path ; \
		print os.path.basename( \
		    distutils.sysconfig.get_python_lib(0,1))'])`
	    cs_cv_python_base_cflags=`AC_RUN_LOG([$PYTHON -c \
		'import distutils.sysconfig ; \
		print "-I"+distutils.sysconfig.get_python_inc()'])`
	    cs_cv_python_base_lflags=`AC_RUN_LOG([$PYTHON -c \
		'import distutils.sysconfig ; import os.path ; \
		print "-L"+distutils.sysconfig.get_python_lib(0,1)+" "+ \
		    "-L"+distutils.sysconfig.get_python_lib(0,1)+"/config"'])`
	    cs_cv_python_base_libs=`AC_RUN_LOG([$PYTHON -c \
		'import distutils.sysconfig ; \
		print distutils.sysconfig.get_config_var("LIBS") + " " + \
		distutils.sysconfig.get_config_var("SYSLIBS")'])`
	    cs_cv_python_base_cflags=CS_TRIM([$cs_cv_python_base_cflags])
	    cs_cv_python_base_lflags=CS_TRIM([$cs_cv_python_base_lflags])
	    cs_cv_python_base_libs=CS_TRIM([$cs_cv_python_base_libs])
	    cs_cv_python_sdk=yes])

	# Not Python 2.x; try Python 1.x.
	AS_IF([test $cs_cv_python_sdk != yes],
	    [cs_python_paths=`AC_RUN_LOG([$PYTHON -c \
'import sys
for p in sys.path: print p'])`
	    AS_IF([test -n "$cs_python_paths"],
		[for cs_pypath in $cs_python_paths
		do
		    cs_pypath=`echo $cs_pypath | sed 's:/$::'`
		    AS_IF([test -f $cs_pypath/config/Makefile],
			[cs_python_lib_dir=$cs_pypath
			cs_python_makefile=$cs_pypath/config/Makefile
			break])
		done])])],

	[# Unable to query missing Python executable or query failed, so try
	# PYTHON_LIB and well-known locations.
	AS_IF([test -n "$PYTHON_LIB"],
	    [cs_python_lib_dir=$PYTHON_LIB
	    AS_IF([test -f $PYTHON_LIB/config/Makefile],
		[cs_python_makefile=$PYTHON_LIB/config/Makefile])],
	    [for cs_pypath in /usr/local/python /usr/local /usr
	    do
		# If multiple Python subdirectories exists in a given path,
		# we choose the last one since it probably has the highest
		# version number (that is, do not "break" from the loop).
		for cs_pydir in $cs_pypath/lib/python*
		do
		    AS_IF([test -f $cs_pydir/config/Makefile],
			[cs_python_lib_dir=$cs_pydir
			cs_python_makefile=$cs_pydir/config/Makefile])
		done
	    done])])

    # If compiler and linker flags not yet determined but resource directory
    # and makefile located, then try extracting information from Python
    # makefile.
    AS_IF([test $cs_cv_python_sdk != yes],
	[AS_IF([test -n "$cs_python_makefile"],
	    [cs_pylibs=`AC_RUN_LOG(
		[grep '^LIBS[[ 	]]*=' $cs_python_makefile | \
		    sed 's/LIBS[[ 	]]*=//'])`
	    cs_pylibm=`AC_RUN_LOG(
		[grep '^LIBM[[ 	]]*=' $cs_python_makefile | \
		    sed 's/LIBM[[ 	]]*=//'])`
	    cs_pylibc=`AC_RUN_LOG(
		[grep '^LIBC[[ 	]]*=' $cs_python_makefile | \
		    sed 's/LIBC[[ 	]]*=//'])`])

	# Construct CFLAGS and LFLAGS
	AS_IF([test -n "$cs_python_lib_dir"],
	    [cs_cv_python_base=`AS_BASENAME([$cs_python_lib_dir])`
	    AS_IF([test -n "$cs_cv_python_base"],
		[AS_IF([test -n "$PYTHON_INC"],
		    [cs_python_inc_dir=$PYTHON_INC],
		    [cs_pyinc=`AS_DIRNAME([$cs_python_lib_dir])`
		     cs_pyinc=`AS_DIRNAME([$cs_pyinc])`
		    cs_python_inc_dir="$cs_pyinc/include/$cs_cv_python_base"])

		cs_cv_python_base_cflags="-I$cs_python_inc_dir"
		cs_cv_python_base_lflags="-L$cs_python_lib_dir
		    -L$cs_python_lib_dir/config"
		cs_cv_python_base_libs="$cs_pylibs $cs_pylibm $cs_pylibc"

		cs_cv_python_base_cflags=CS_TRIM([$cs_cv_python_base_cflags])
		cs_cv_python_base_lflags=CS_TRIM([$cs_cv_python_base_lflags])
		cs_cv_python_base_libs=CS_TRIM([$cs_cv_python_base_libs])
		cs_cv_python_sdk=yes])])])])

# Check if Python SDK is usable.  Some distributions (such as the one shipped
# with MacOS/X Jaguar which is missing the static link library) are broken.
# The most common library name is the basename with a few decorations (for
# example, "libpython2.2.a"), however some Windows libraries lack the decimal
# point (for example, "libpython22.a" or "python22.lib"), so we must check for
# both variations.
cs_pylib1="-l$cs_cv_python_base"
cs_pylib2=`echo "-l$cs_cv_python_base" | sed 's/\.//g'`
CS_CHECK_BUILD([if python SDK is usable], [cs_cv_python],
    [AC_LANG_PROGRAM([#include <Python.h>],[Py_Initialize(); Py_Finalize();])],
    [CS_CREATE_TUPLE([],[],[$cs_pylib1]) CS_CREATE_TUPLE([],[],[$cs_pylib2])],
    [], [CS_EMIT_BUILD_RESULT_PYTHON([cs_cv_python], [PYTHON])], [], [],
    [$cs_cv_python_base_cflags], [$cs_cv_python_base_lflags],
    [$cs_cv_python_base_libs])

AS_IF([test "$PYTHON" = yes],[
  ifelse([$1], , :, [$1])
],[
  ifelse([$2], , :, [$2])
])

])

# helper
AC_DEFUN([CS_EMIT_BUILD_RESULT_PYTHON],
    [AS_IF([test "$$1" = yes],
        [CS_JAMCONFIG_PROPERTY([$2.AVAILABLE], [yes])
        cs_result_cflags=CS_TRIM([$$1_cflags])
        cs_result_lflags=CS_TRIM([$$1_lflags $$1_libs])
        AS_IF([test -n "$cs_result_cflags"],
            [CS_JAMCONFIG_PROPERTY([$2.CFLAGS], [$cs_result_cflags])])
        AS_IF([test -n "$cs_result_lflags"],
            [CS_JAMCONFIG_PROPERTY([$2.LFLAGS], [$cs_result_lflags])])])])
