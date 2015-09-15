# - MACRO_ADD_TEST(<args>)
#
# Calls add_test() with all the <args> but if on Win32 or Cygwin also adds the
# directory where the Cgreen library is generated so that it will be used when
# running the test
#
# @thoni56/Thomas Nilsson 2015-09-13

macro (macro_add_test)
  add_test(${ARGN})
  if (CYGWIN OR WIN32)
    set_tests_properties(${ARGV1} PROPERTIES ENVIRONMENT PATH=$ENV{PATH}:${CMAKE_BINARY_DIR}/src)
  endif ()
endmacro(macro_add_test)
