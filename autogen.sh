#! /bin/sh

# Correct working directory?
if test ! -f configure.ac ; then
  echo "*** Please invoke this script from directory containing configure.ac."
  exit 1
fi

MACROFILE=aclocal.m4
MACRODIR=mk/autoconf
                                                                                
# Creating normal configure file
rm -f $MACROFILE
for i in $MACRODIR/*.m4 ; do
  cat $i >> $MACROFILE
done
autoconf
rm -f $MACROFILE

