dnl
dnl Copyright (C) 2002 Loic Dachary (loic@gnu.org)
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
dnl
dnl
dnl AM_PATH_CS([minimum version])
dnl
dnl Figure out the location of the crystalspace headers and libraries
dnl directories. LDFLAGS and CPPFLAGS are filled with the values found.
dnl
dnl
AC_DEFUN(AM_PATH_CS,
[ 
AC_ARG_WITH( cs,
    [  --with-cs=<path>       path to the crystalspace install root.
                          e.g. /usr/local])

AC_ARG_WITH( cs-include,
    [  --with-cs-include=<path>
                          path to the crystalspace headers directory.
                          e.g. /usr/local/include])

AC_ARG_WITH( cs-lib,
    [  --with-cs-lib=<path>
                          path to the crystalspace libraries/plugins directory.
                          e.g. /usr/local/cs/lib])

dnl Check for cs-config
AC_PATH_PROG(CS_CONFIG, cs-config, no)

dnl 
dnl Configure options (--with-cs*) have precendence 
dnl over cs-config only set variables if they are not 
dnl specified
dnl
if test "$CS_CONFIG" != "no"
then
    if test "$1"
    then
    	min_version=$1
    	current_version=`$CS_CONFIG --version`
	if test `echo $min_version | tr -d .` -gt `echo $current_version | tr -d .`
	then
		AC_MSG_ERROR([crystalspace version $current_version, need at least $min_version])
	else
		AC_MSG_NOTICE([crystalspace version $current_version])
	fi
    fi
    if test -z "$with_cs" -a -z "$with_cs_include"
    then
	with_cs_include=`$CS_CONFIG --includedir`
    fi

    if test -z "$with_cs" -a -z "$with_cs_lib"
    then
	with_cs_lib=`$CS_CONFIG --libdir`
    fi
else
    AC_MSG_WARN([cs-config script not found, unable to check that version is greater or equal to $1 ])
fi

dnl
dnl Set cs_libraries and cs_includes according to
dnl user specification (--with-cs*) if any. 
dnl --with-cs-include and --with-cs-lib have precendence
dnl over --with-cs
dnl
if test "$with_cs" = "no"
then
    dnl The user explicitly disabled the use of the Cs
    AC_MSG_ERROR([crystalspace is mandatory: do not specify --without-cs])
else
    if test "$with_cs" -a "$with_cs" != "yes"
    then
	cs_includes="$with_cs/include"
	cs_libraries="$with_cs/lib"
    fi
fi

if test "$with_cs_include"
then
    cs_includes="$with_cs_include"
fi

if test "$with_cs_lib"
then
    cs_libraries="$with_cs_lib"
fi

dnl
dnl Set compilation variables 
dnl
if test "$cs_includes"
then
    AC_MSG_NOTICE([crystalspace headers directory $cs_includes])
    CPPFLAGS="$CPPFLAGS -I$cs_includes"
fi

if test "$cs_libraries"
then
    AC_MSG_NOTICE([crystalspace libraries directory $cs_libraries])
    LDFLAGS="-L$cs_libraries $LDFLAGS"
fi

AC_LANG_PUSH(C++)

AC_CHECK_HEADER([cssysdef.h],,[AC_MSG_ERROR(try using --with-cs to specify the location 
         of crystalspace header files)])
AC_CHECK_LIB(cssys,main,,[AC_MSG_ERROR(try using --with-cs to specify the location
         of crystalspace libraries)])

AC_LANG_POP()

])
