#!/bin/sh
if [ -x all_tests ]
then
   echo -n "Some stuff" > tests/some_file
   ./all_tests
else
   echo "Error: must 'make check' first"
   exit 1
fi
