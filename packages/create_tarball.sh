#!/bin/sh
cd ../..
tar -zcf cgreen.tar.gz cgreen/LICENSE \
                         cgreen/makefile \
                         cgreen/*.c \
                         cgreen/*.h \
                         cgreen/tests/*.c \
                         cgreen/tests/makefile \
                         cgreen/samples/*.c \
                         cgreen/samples/makefile \
                         cgreen/README
