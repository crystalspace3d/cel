dnl a macro that search for the libpython
AC_DEFUN([AM_CHECK_PYTHON_LIBS],
[
AC_REQUIRE([AM_PATH_PYTHON])
AC_REQUIRE([AM_CHECK_PYTHON_HEADERS])
AC_MSG_CHECKING(for python libraries)
dnl py_prefix=`$PYTHON -c "import sys; print sys.prefix"`
py_exec_prefix=`$PYTHON -c "import sys; print sys.exec_prefix"`
PYTHON_LIBS="-L${py_exec_prefix}/lib/python${PYTHON_VERSION} -L${py_exec_prefix}/lib/python${PYTHON_VERSION}/config -lpython${PYTHON_VERSION}"

AC_SUBST(PYTHON_LIBS)
dnl do a test compile
save_CFLAGS="$CPPFLAGS"
save_LIBS="$LIBS"
CFLAGS="$CFLAGS $PYTHON_CFLAGS"
LIBS="$LIBS $PYTHON_LIBS"
FOUND=0
AC_TRY_LINK([#include <Python.h>],[Py_Initialize();],[FOUND=1],[FOUND=0])
dnl no follows a nasty hack for people having only libpython.a
if test $FOUND = 0; then
	PYTHON_LIBS="$PYTHON_LIBS -ldl -lpthread -lutil -lm"
	LIBS="$save_LIBS $PYTHON_LIBS"
	AC_TRY_LINK([#include <Python.h>],[Py_Initialize();],[FOUND=1],[FOUND=0])
fi

if test $FOUND = 1; then
	AC_MSG_RESULT(found)
	ifelse([$1], , :, [$1])
else
	PYTHON_LIBS=""
	AC_MSG_RESULT(not found)
	ifelse([$2], , :, [$2])
fi
LIBS="$save_LIBS"
CFLAGS="$save_CFLAGS"
])

