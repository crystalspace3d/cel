#
# Copyright (C) 2002 Loic Dachary (loic@gnu.org)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# Process this file with autoconf to produce a configure script.

AC_INIT(cel, 0.1.0, cel-main@lists.sourceforge.net)

AC_PREREQ(2.53)

AM_INIT_AUTOMAKE(cel,AC_PACKAGE_VERSION)

AC_CONFIG_SRCDIR([plimp/entity.cpp])
AM_CONFIG_HEADER(include/celconfig.h)

AC_SUBST(VERSION)

LIBTOOL_VERSION=`echo AC_PACKAGE_VERSION | tr . :`
AC_SUBST(LIBTOOL_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_PROG_CXXCPP
AC_PROG_CPP

AM_DISABLE_STATIC
AM_PROG_LIBTOOL

AM_SANITY_CHECK

AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset pow sqrt strchr])

#
# Crystalspace dependencies
#
AM_PATH_CS(0.93)

AC_LANG(C++)

AC_CHECK_LIB(csutil, main,,AC_MSG_ERROR([missing library]))
AC_CHECK_LIB(csgeom, main,,AC_MSG_ERROR([missing library]))
AC_CHECK_LIB(csgfx, main,,AC_MSG_ERROR([missing library]))
AC_CHECK_LIB(cstool, main,,AC_MSG_ERROR([missing library]))

LDFLAGS="`cs-config --libs python` $LDFLAGS"
CPPFLAGS="`cs-config --cxxflags python` $CPPFLAGS"

#
# Search for includes from the top level source directory
#
CPPFLAGS="$CPPFLAGS -I\${top_srcdir} -I\${top_srcdir}/include"

CPPFLAGS="$CPPFLAGS `cs-config --cxxflags`"

AC_CONFIG_FILES([
	Makefile
	plimp/Makefile
	persist/Makefile
	persist/classic/Makefile
	bltest/Makefile
	blpython/Makefile
	celtest/Makefile
	pf/Makefile
	pf/common/Makefile
	pf/mesh/Makefile
	pf/engine/Makefile
	pf/input/Makefile
	pf/inv/Makefile
	pf/move/Makefile
	pf/test/Makefile
	pf/tools/Makefile
	include/Makefile
	include/bl/Makefile
	include/pf/Makefile
	include/pl/Makefile
	scripts/Makefile
	scripts/cel-config
	])
AC_OUTPUT
