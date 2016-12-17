#! /bin/sh
#
# When you run "make test" CMake obviously runs the correct binraries
# from the build directory. But when you want to run a test manually
# PATH and LD_LIBRARY_PATH need to be set, and it is tedious and error
# prone to remember that all the time.
#
# The follwing sets up the current shell in the same way, so that a
# developer can just source this script and then forget about it.
#

# Ensure that the script is sourced:
if [[ $_ == $0 ]]; then
   echo "You should really source this, like in '. cygwin-setup.sh'"
fi

# And here's the meat given that you have the standard build tree
export PATH="$PWD/build/tools":$PATH
export LD_LIBRARY_PATH="$PWD/build/src":$PATH
