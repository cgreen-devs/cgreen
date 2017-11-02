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

# Add the appropriate directories in the build tree to PATH et al.
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

# Let's also create symbolic links to build/tools and build/src to emulate typical
# FHS include/bin/lib structure. This makes it easy to switch between various versions of
# cgreen. E.g. in a Makefile you can do
#
# CGREEN_ROOT=/usr/local
# CGREEN_INCLUDE=$(CGREEN_ROOT)/include
# CGREEN_LIB=$(CGREEN_ROOT)/lib
# CGREEN_BIN=$(CGREEN_ROOT)/bin
# ...
#     LD_LIBRARY_PATH=$(CGREEN_LIB):. $(CGREEN_BIN)/bin/cgreen-runner <libs>
# ...
# %.so: %.c
#	g++ -o $@ -shared -g -I$(CGREEN_INCLUDE) -fPIC $^ -L $(CGREEN_LIBDIR) -lcgreen
#
# To switch to your current development version just change CGREEN_ROOT.
#
# So simulate that build/tools (where cgreen-runner is) is bin, and build/src (where
# the cgreen library is built) is lib
[ -e bin ] || ln -s build/tools bin
[ -e lib ] || ln -s build/src lib
