# AC_PATH_CEL (c) 2003 by Matthias Braun <matze@braunis.de>
#
#
# Checks for Cel paths and libs,
# This scripts tries first if it can find a cs-config in the actual path
# if yes it just uses that. If not it looke if CEL var is set.
# The script will set the CEL_AVAILABLE, CEL_VERSION, CEL_LIBS and CEL_CFLAGS
# variables.
AC_DEFUN([CS_PATH_CEL_HELPER],
[
AC_ARG_WITH(cel-prefix, AC_HELP_STRING([--with-cel-prefix=PFX], [Prefix where to Cel is installed (optional)]),
    [CELPREFIX="$withval"], [CELPREFIX=""])
AC_ARG_ENABLE(cel-test, AC_HELP_STRING([--disable-cel-test], [Do not try to compile and run a cel test program]), enable_celtest="$enableval",
        enable_celtest="no")

no_cel=no

if test -z "$CELPREFIX"; then
    AC_CHECK_PROGS(CELCONFIG, cel-config, "")
    if test -z "$CELCONFIG"; then
        AC_CHECK_PROGS(CELCONFIG, cel-config, "", $CEL)
        if test -n "$CELCONFIG"; then
            CELCONFIG="$CEL/cel-config"
        fi
    fi
else
    AC_CHECK_PROGS(CELCONFIG, cel-config, "", $CELPREFIX/bin)
fi

if test -z "$CELCONFIG"; then
    AC_MSG_WARN([Can't find cel-config script])
    no_cel=yes
fi

if test "$no_cel" = "no" ; then
    AC_MSG_CHECKING([for CEL - version >= $1])
    CEL_CFLAGS=`$CELCONFIG --cflags`
    CEL_LIBS=`$CELCONFIG --libs`
    CEL_VERSION=`$CELCONFIG --version`
    CEL_AVAILABLE=yes
    
    if test -z "$CEL_VERSION"; then
        AC_MSG_RESULT(no)
        no_cs=yes
    fi
fi

if test "$no_cs" = "yes"; then
    enable_celtest=no
fi

if test "$enable_celtest" != "no"; then
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    ac_save_CXXFLAGS="$CXXFLAGS"
    ac_save_LIBS="$LIBS"

    CXXFLAGS="$CXXFLAGS $CRYSTAL_CFLAGS $CEL_CFLAGS"
    LIBS="$LIBS $CRYSTAL_LIBS $CEL_LIBS"

    AC_TRY_RUN([
#include <cssysdef.h>

#include <physicallayer/entitiy.h>
            
CS_IMPLEMENT_APPLICATION

int main(int argc, char** argv)
{
    // TODO a nice testapp...
    return 0;
}
],, [no_cel=yes
AC_MSG_RESULT(no)], [echo "$ac_n cross compiling; assumed OK. $ac_c"])

    CXXFLAGS="$ac_save_CXXFLAGS"
    LIBS="$ac_save_LIBS"
    AC_LANG_RESTORE
fi

if test "$no_cel" = "no"; then
    AC_MSG_RESULT($CEL_VERSION)
    ifelse([$2], , :, [$2])
else
    CEL_CFLAGS=""
    CEL_VERSION=""
    CEL_LIBS=""
    CEL_AVAILABLE="no"
    ifelse([$3], , :, [$3])
fi

])

dnl AC_PATH_CEL([Minimum-version, [ACTION-IF_FOUND, [ACTION-IF-NOT-FOUND]]])
dnl Test for cel and sets CEL_VERSION, CEL_CFLAGS, CEL_LIBS and CEL_AVAILABLE
dnl variables (this version uses AC_SUBST for the results)
AC_DEFUN([AC_PATH_CEL], [

CS_PATH_CEL_HELPER($1,$2,$3)

AC_SUBST([CEL_CFLAGS])
AC_SUBST([CEL_LIBS])
AC_SUBST([CEL_VERSION])
AC_SUBST([CEL_AVAILABLE])

])

dnl CS_PATH_CEL([Minimum-version, [ACTION-IF-FOUND, [ACTION-IF-NOT-FOUND]]])
dnl Test for cel and sets CEL_VERSION, CEL_CFLAGS, CEL_LIBS and CEL_AVAILABLE
dnl variables (this version uses CS_SUBST for the results)
AC_DEFUN([CS_PATH_CEL], [

CS_PATH_CEL_HELPER($1,$2,$3)

CS_JAMCONFIG_PROPERTY([CEL_CFLAGS], [$CEL_CFLAGS])
CS_JAMCONFIG_PROPERTY([CEL_LIBS], [$CEL_LINS])
CS_JAMCONFIG_PROPERTY([CEL_VERSION], [$CEL_VERSION])
CS_JAMCONFIG_PROPERTY([CEL_AVAILABLE], [$CEL_AVAILABLE])

])

