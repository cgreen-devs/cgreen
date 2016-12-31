#! /bin/sh
#
# Setup PATH and LD_LIBRARY_PATH for command line work
#
# When you run "make test" CMake obviously runs the correct binaries
# from the build directory. And so does the top level Makefile.  But
# when you want to run a test manually PATH and LD_LIBRARY_PATH need
# to be set, and it is tedious and error prone to remember that all
# the time.
#
# The follwing sets up the current shell in the same way, so that a
# developer can just source this script and then always get the ones
# in the build tree wihtout thinking about it.
#

# Ensure that the script is sourced:
if [ $_ = $0 ]; then
   echo "You should really source this, like in '. $0'"
fi

contains() {
    string="$1"
    substring="$2"
    if test "${string#*$substring}" != "$string"
    then
        return 0    # $substring is in $string
    else
        return 1    # $substring is not in $string
    fi
}

if contains "$PATH" "$PWD/build/tools" ; then
    echo "You already have the appropriate directories in your PATH and LD_LIBRARY_PATH."
else

    # And here's the meat given that you have the standard build tree
    export PATH="$PWD/build/tools":$PATH

    # On linux we need LD_LIBRARY_PATH...
    export LD_LIBRARY_PATH="$PWD/build/src":$PATH

    # ...but on Cygwin DLL:s are searched using the path...
    export PATH="$PWD/build/src":$PATH
fi
