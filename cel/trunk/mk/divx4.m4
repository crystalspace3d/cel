# AC_PATH_DIVX4
#
# Looks for opendivx4 plugin
#
# Most of this code comes from avifile configure.in (C) Eugene Kuznetsov, Zdenek Kabelac
# macro done by Matze Braun <MatzeBraun@gmx.de>

dnl AC_PATH_DIVX4([ACTION-IF-FOUND [, ACTION-IF_NOT-FOUND ]])

AC_DEFUN(AC_PATH_DIVX4,
[
AC_ARG_WITH(divx4-prefix, AC_HELP_STRING([--with-divx4-prefix=PFX], [Prefix where opendivx4 is installed (optional)]),
	,with_divx4_prefix=)

AC_ARG_ENABLE(divx4, AC_HELP_STRING([--enable-divx4],[build divx4 plugin. (autodetect)]),
	,enable_divx4=yes)


if test x$enable_divx4 = xyes ; then
    have_divx_decore=no
    have_divx_encore=yes
    DIVX4_CFLAGS=""
    DIVX4_LIBS=""
    ac_cppflags_save=$CPPFLAGS
    if test -n "$with_divx4_prefix" ; then
        dnl user has specified extra path for DivX4 instalation
        CPPFLAGS="-I$with_divx4_prefix $CPPFLAGS"
        AC_CHECK_HEADER(include/decore.h,
                        DIVX4_CFLAGS="-I$with_divx4_prefix/include"
                        DIVX4_LIBS="-L$with_divx4_prefix/lib"
                        have_divx_decore=yes)
        if test x$have_divx_decore = xno ; then
            CPPFLAGS="-I$with_divx4_prefix $CPPFLAGS"
            AC_CHECK_HEADER(decore.h,
                            DIVX4_CFLAGS="-I$with_divx4_prefix"
                            DIVX4_LIBS="-L$with_divx4_prefix"
                            have_divx_decore=yes)
        fi
    else
        AC_CHECK_HEADER(decore.h, have_divx_decore=yes)
    fi

    AC_CHECK_HEADER(encore2.h, , have_divx_encore=no)
    CPPFLAGS=$ac_cppflags_save
	
    ac_libs_save=$LIBS
    if test x$have_divx_decore = xyes ; then
        AC_CHECK_LIB(divxdecore, decore,
                     AC_DEFINE([HAVE_LIBDIVXDECORE], 1, [Define if you have 'divxdecore' library (-ldivxdecore).])
                     DIVX4_LIBS="$DIVX4_LIBS -ldivxdecore",
                     have_divx_decore=no, $DIVX4_LIBS)
    else
        enable_divx4="no "
    fi
    if test x$have_divx_encore = xyes ; then
        AC_CHECK_LIB(divxencore, encore,
                     AC_DEFINE([HAVE_LIBDIVXENCORE], 1, [Define if you have 'divxencore' library (-ldivxencore).])
                     DIVX4_LIBS="$DIVX4_LIBS -ldivxencore",
                     have_divx_encore=no, $DIVX4_LIBS)
    fi
    LIBS=$ac_libs_save
    if test x$have_divx_decore = xyes ; then
    	ifelse([$1], , :, [$1])
    else
    	ifelse([$2], , :, [$2])
    fi
    AC_SUBST(DIVX4_CFLAGS)
    AC_SUBST(DIVX4_LIBS)
fi
])

