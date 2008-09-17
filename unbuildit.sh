#!/bin/bash
find . -name Makefile.in -exec rm -v {} \;
find . -name Makefile -exec rm -v {} \;
rm aclocal.m4 config.guess config.sub configure install-sh ltconfig ltmain.sh missing mkinstalldirs depcomp
rm Makefile config.cache config.log config.status libtool
rm config.h.in stamp-h.in
