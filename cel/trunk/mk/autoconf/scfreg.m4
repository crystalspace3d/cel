#-----------------------------------------------------------------------------
# try to find CS scfreg application
#   and sets the SCFREG variable.
#-----------------------------------------------------------------------------

dnl CS_PROG_SCFREG([ACTION-IF-FOUND,[ACTION-IF-NOT-FOUND]])
AC_DEFUN([CS_PROG_SCFREG],[
  AC_REQUIRE([CS_PATH_CRYSTAL_HELPER])
  AC_CHECK_PROGS([SCFREG], [scfreg])
  AS_IF([test -z "$SCFREG"],
    [AC_MSG_CHECKING([for scfreg in CRYSTAL])
     AS_IF([test -x "$CRYSTAL/bin/scfreg"], [SCFREG="$CRYSTAL/bin/scfreg"])
     AS_IF([test -x "$CRYSTAL/scfreg"], [SCFREG="$CRYSTAL/scfreg"])
     AS_IF([test -n "$SCFREG"],[AC_MSG_RESULT(found)])
    ])
  AS_IF([test -n "$SCFREG"], [ifelse([$1], , :, [$1])],
			[ifelse([$2], , :, [$2])])
])
