# doxygen.m4                                               -*- Autoconf -*-
#------------------------------------------------------------------------------
dnl CHECK_DOXYGEN([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN([CHECK_DOXYGEN],
[
  AC_CHECK_PROGS([DOXYGEN], [doxygen])
  AS_IF([test -n "$DOXYGEN"], [CS_JAMCONFIG_PROPERTY([CMD.DOXYGEN], [$DOXYGEN])])
])
