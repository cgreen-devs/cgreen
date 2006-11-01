#!/bin/sh
cd ../..
tar -zcf cgreen_`cat cgreen/VERSION`.tar.gz \
                         cgreen/LICENSE \
                         cgreen/VERSION \
                         cgreen/makefile \
                         cgreen/*.c \
                         cgreen/*.h \
                         cgreen/tests/*.c \
                         cgreen/tests/makefile \
                         cgreen/samples/*.c \
                         cgreen/samples/makefile \
                         cgreen/documentation/*.html \
                         cgreen/documentation/docs.css \
                         cgreen/README
