#! /bin/sh
#
# I'm making some notes here for future reference: CYGWIN is *not*
# WIN32, but it *is* a DLL-platform So it loads DLL:s the same way
# Windows does, namely from the path. That means that for tests to run
# using the freshly build DLL:s in the build tree both include paths
# but also the PATH need to be prepended by the directory of the
# DLL. This could be done with something like:
#
#   set_tests_properties(SomeTest PROPERTIES ENVIRONMENT "PATH=c:\somedir;c:\otherdir")
#
# which I found at http://stackoverflow.com/a/1079072/204658. The
# thing to work out is what CMAKE variables point to that
# directory. The follwing should be evolved to do just that, so that a
# CYGWIN developer could just source this script and then forget about
# it.
#

# Ensure that the script is sourced:
if [[ $_ == $0 ]]; then
   echo "You should really source this, like in '. cygwin-setup.sh'"
fi

# And here's the meat
export PATH="$PWD/build/build-c/src":"$PWD/build/build-c++/src":$PATH
export PATH="$PWD/build/build-c/tools":"$PWD/build/build-c++/tools":$PATH
