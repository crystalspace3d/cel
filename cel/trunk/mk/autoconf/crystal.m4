# AC_PATH_CRYSTAL
#
# Checks for Crystal Space paths and libs,
# This scripts tries first if it can find a cs-config in the actual path
# if yes it just uses that. If not it looke if CRYSTAL var is set, and after
# that if it tries to find Crystal Space and then it just looks in 
# /usr/local/crystal. Causes an error if it cant find it or you fed it a bad
# (optional) path. Remember to do CFLAGS="$CFLAGS $CRYSTAL_CFLAGS" and so
# forth for CRYSTAL_LIBS, CRYSTAL_CFLAGS, and CRYSTAL_LIBS so you can use
# the information provided by this script!

# Matze Braun <MatzeBraun@gmx.de>
# Patrick McFarland (Diablo-D3) <unknown@panax.com>

dnl helper function
AC_DEFUN(CS_PATH_CRYSTAL_HELPER,
[dnl 
dnl Get the cflags and libraries from the cs-config script
dnl
AC_ARG_WITH(cs-prefix, AC_HELP_STRING([--with-cs-prefix=PFX],[Prefix where Crystal Space is installed (optional)]), CRYSTAL="$withval", )
AC_ARG_ENABLE(cstest, AC_HELP_STRING([--disable-cstest],[Do not try to compile and run a test Crystal Space program]), , enable_cstest=yes)
AC_ARG_VAR([CRYSTAL],     [Prefix Where Crystal Space is installed])
dnl if you really want to use autoconf 2.13 (not recommended) comment out the
dnl line above.

dnl try to find an installed cs-config

if test "x$CRYSTAL" != x ; then
   if test -f $CRYSTAL/cs-config ; then
      CSCONF="$CRYSTAL/cs-config"
   else
      if test -f $CRYSTAL/bin/cs-config ; then
         CSCONF="$CRYSTAL/bin/cs-config"
      else
         AC_MSG_WARN(Can't find cs-config in path you provided)
	 no_cs=yes
      fi
   fi	
fi

if test "x$CRYSTAL" = x ; then
   AC_CHECK_PROGS([CSCONF], [cs-config])
   if test "x$CSCONF" = x ; then 
      CRYSTAL=/usr/local/crystal
      if test -f $CRYSTAL/bin/cs-config ; then
         CSCONF="$CRYSTAL/bin/cs-config"
      else
	 no_result=yes
	 no_cs=yes
      fi
   fi
fi

# Well, either we found cs by now, or we caused an error.
# Now we define stuff, then check if we are running a new enough version

if test "x$no_cs" = "x" ; then
	min_cs_version=ifelse([$1], ,0.94,$1)
	AC_MSG_CHECKING(for Crystal Space - version >= $min_cs_version)
	CRYSTAL_CFLAGS=`$CSCONF $csconf_args --includes $4`
	CRYSTAL_LIBS=`$CSCONF $csconf_args --libs $4`
	CRYSTAL_VERSION=`$CSCONF --version $4`

	cs_major_version=`$CSCONF $cs_args --version | \
	   sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)?/\1/'`
	cs_minor_version=`$CSCONF $cs_args --version | \
	   sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)?/\2/'`

	if test "x$CRYSTAL_LIBS" = "x" ; then
		no_cs=yes
	fi
fi

if test "x$no_cs" != "x" ; then
	enable_cstest=no
fi

if test x$enable_cstest = xyes ; then
   ac_save_CXXFLAGS="$CXXFLAGS"
   ac_save_LIBS="$LIBS"
   CXXFLAGS="$CXXFLAGS $CRYSTAL_CFLAGS"
   LIBS="$LIBS $CRYSTAL_LIBS"

   AC_LANG_SAVE()
   AC_LANG(C++)
   AC_TRY_RUN([
#include <cssysdef.h>

#include <stdio.h>

#include <csutil/util.h>

CS_IMPLEMENT_APPLICATION

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  /* HP/UX 9 writes to sscanf strings */
  tmp_version = csStrNew ("$min_cs_version");
  if (sscanf (tmp_version, "%d.%d", &major, &minor) != 2)
  {
     printf("%s, bad version string\n", "$min_cs_version");
     return 1;
  }

  if (($cs_major_version > major) ||
     (($cs_major_version == major) && ($cs_minor_version > minor)) ||
     (($cs_major_version == major) && ($cs_minor_version == minor)))
  {
    return 0;
  }
  else
  {
    printf("\n*** 'cs-config --version' returned %d.%d, but the minimum version\n", $cs_major_version, $cs_minor_version);
    printf("*** of Crystal Space required is %d.%d. If cs-config is correct, then it is\n", major, minor);
    printf("*** best to upgrade to the required version.\n");
    printf("*** If cs-config was wrong, set the environment variable CRYSTAL\n");
    printf("*** to point to the correct copy of cs-config, and remove the file\n");
    printf("*** config.cache before re-running configure\n");
    return 1;
  }
}

],, no_cs=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
  CXXFLAGS="$ac_save_CXXFLAGS"
  LIBS="$ac_save_LIBS"
  AC_LANG_RESTORE()
fi

if test "x$no_cs" = "x" ; then
   AC_MSG_RESULT($CRYSTAL_VERSION)
   ifelse([$2], , :, [$2])     
else
   if test x$no_result = x ; then
     AC_MSG_RESULT(no)
   fi
   CRYSTAL_CFLAGS=""
   CRYSTAL_VERSION=""
   CRYSTAL_LIBS=""
   ifelse([$3], , :, [$3])
fi

])

dnl CS_PATH_CRYSTAL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, LIBS]]]])
dnl Test for CS, and define CRYSTAL_VERSION, 
dnl CRYSTAL_CFLAGS, and CRYSTAL_LIBS
dnl
AC_DEFUN([CS_PATH_CRYSTAL],[

CS_PATH_CRYSTAL_HELPER($1,$2,$3,$4)

AC_SUBST([CRYSTAL_CFLAGS])
AC_SUBST([CRYSTAL_LIBS])
AC_SUBST([CRYSTAL_VERSION])

])

dnl CS_PATH_CRYSTAL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, LIBS]]]])
dnl Test for CS, and define CRYSTAL_VERSION, 
dnl CRYSTAL_CFLAGS, and CRYSTAL_LIBS (just sets JAMCONFIG properties instead of
dnl Makefile substs.
dnl
AC_DEFUN([CS_PATH_CRYSTAL_JAM],[

CS_PATH_CRYSTAL_HELPER($1,$2,$3,$4)

CS_JAMCONFIG_PROPERTY([CRYSTAL.CFLAGS], [$CRYSTAL_CFLAGS])
CS_JAMCONFIG_PROPERTY([CRYSTAL.LFLAGS], [$CRYSTAL_LIBS])
CS_JAMCONFIG_PROPERTY([CRYSTAL.VERSION], [$CRYSTAL_VERSION])

])

