# AC_PROG_JAM
#
# Checks if jam/mr is installed. If the script can't find jam some usefull
# informations are displayed for the user.
#
# (c)2002 Matze Braun <MatzeBraun@gmx.de>
dnl AC_PROG_JAM()

AC_DEFUN(AC_PROG_JAM,
[
AC_ARG_ENABLE(jamtest, AC_HELP_STRING([--disable-jamtest],
	[Do not try to check for jam/mr tool]), ,enable_jamtest=yes )

if test x$enable_jamtest = xyes ; then
  AC_CHECK_TOOL(JAM, jam)
  if test "x$JAM" = x ; then
    AC_MSG_ERROR([
*** Couldn't find the jam build tool. This package requires the jam/mr tool
*** for building. You can find the tool and more informations at:
***    http://www.perforce.com/jam/jam.html
*** (you can disable this check with --disable-jamtest)
  ])
  fi
fi

])

