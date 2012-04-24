#!/bin/sh

# Eric's poor-man (portable) 'dirname' command.
dirpart() {
  dirpart_d=`expr "$1" : '\(..*\)[/\\].*'`
  if test -n "${dirpart_d}"; then
    echo ${dirpart_d}
  else
    echo '.'
  fi
}

mydir=`dirpart $0`
./bootstrap cel.behaviourlayer.python ${mydir}/scripts/smallgame/smallgame $*
