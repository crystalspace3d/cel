AC_DEFUN([AC_PATH_PYTHON],
[
AM_PATH_PYTHON([$1])
AM_CHECK_PYTHON_HEADERS()
AM_CHECK_PYTHON_LIBS([$2],[$3])
])