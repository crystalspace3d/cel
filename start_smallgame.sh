#!/bin/sh
myPath=`echo $0 | sed -e "s/\/.*//"`
./bootstrap cel.behaviourlayer.python ${myPath}/scripts/smallgame/smallgame $*
