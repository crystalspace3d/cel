#!/bin/sh
#
set -e

# Refresh GNU autotools toolchain.
aclocal -I mk
libtoolize --force --copy
autoheader
automake --copy --verbose --foreign --add-missing
autoconf

exit 0
