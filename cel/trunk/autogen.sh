#!/bin/sh

MACROFILE=aclocal.m4
echo "creating $MACROFILE from macros out of mk/*.m4"
rm -f $MACROFILE
for i in `find mk/ -name "*.m4"`; do
	cat $i >> $MACROFILE
done
echo autoconf
autoconf

