dnl Process this file with autoconf to produce a configure script.
AC_INIT(cel, 0.1)

AC_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_CXX

dnl Check for libs
dnl Check for Crystal Space
AC_PATH_CRYSTAL(0.94,,
  AC_MSG_ERROR([
*** Crystal Space couldn't be found. The latest version is always avaiable
*** from http://crystal.linuxgames.com. 
***
*** be also sure that you've either installed CS or set the CRYSTAL var right.
*** You also have to compile the cs-config script in CS. Do this with the
*** following command in your CS dir:
***	make csconfig]), csengine csgeom csgfx csutil cstool cssys)

AC_PATH_PYTHON(1.5,,
  AC_MSG_WARN([
  *** Python not found -> Python support disabled.
  *** You need at least python 1.5 to compile this package.
  *** If you installed a binary version of python make sure you installed an
  *** eventually existing development package as well.
  *** You can find python and more informations at
  ***    http://www.python.org]))

AC_OUTPUT([
  Jamfile
  Jamrules
])

