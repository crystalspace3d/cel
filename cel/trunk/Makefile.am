#
# Copyright (C) 2002 Loic Dachary (loic@gnu.org)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
AUTOMAKE_FLAGS = --foreign

MAINTAINERCLEANFILES = Makefile.in configure libtool config.guess \
                       config.sub aclocal.m4 config.log config.status \
                       install-sh missing mkinstalldirs ltmain.sh \
		       include/stamp-h.in depcomp \
		       include/celconfig.h include/celconfig.h.in

EXTRA_DIST = cel.cex autogen.sh cel.dsw msvc

SUBDIRS = plimp persist bltest blpython pf include scripts celtest

sysconf_DATA = cel.cex

dist-hook:
	find $(distdir) -name CVS -print | xargs rm -fr
