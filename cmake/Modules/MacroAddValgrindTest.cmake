# - MACRO_ADD_VALGRIND_TEST(<args>)
#
# Calls add_test() with all the <args> but if on Win32 or Cygwin also adds the
# directory where the Cgreen library is generated to the path so that it will
# be used when running the test
#
# @thoni56/Thomas Nilefalk 2015-09-13

macro (macro_add_valgrind_test)
  if (Valgrind_FOUND)
    set(
      libname
      ${CMAKE_FIND_LIBRARY_PREFIXES}${ARGN}${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
    add_test(
      NAME valgrind_${libname}
      COMMAND sh -c "LD_LIBRARY_PATH=build/src valgrind --leak-check=full tools/cgreen-runner ${CMAKE_CURRENT_BINARY_DIR}/${libname} 2>1&"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_tests_properties(
      valgrind_${libname} PROPERTIES
      FAIL_REGULAR_EXPRESSION "(definitely|indirectly|possible) lost: [1-9]"
    )
    if (CYGWIN OR WIN32)
      set_tests_properties(${ARGV1} PROPERTIES ENVIRONMENT PATH=${PROJECT_BINARY_DIR}/src:$ENV{PATH})
    endif ()
  endif ()
endmacro(macro_add_valgrind_test)
