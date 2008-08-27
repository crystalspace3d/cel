#!/bin/sh

OLDDIR=`pwd`
WRAPPERNAME=celstart-launcher-$1
TMPDIR=`mktemp -d`

mkdir -p $TMPDIR/$WRAPPERNAME
cp celstart-launcher $TMPDIR/$WRAPPERNAME
echo "# $WRAPPERNAME" >> $TMPDIR/$WRAPPERNAME/celstart-launcher
cd $TMPDIR
tar --use-compress-program lzma -cf $OLDDIR/$WRAPPERNAME.tar.lzma $WRAPPERNAME
cd $OLDDIR

#rm -rf $TMPDIR
