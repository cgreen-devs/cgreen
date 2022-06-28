if (NOT Valgrind_FOUND)

  find_program(Valgrind_EXECUTABLE valgrind)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Valgrind DEFAULT_MSG Valgrind_EXECUTABLE)

  set(Valgrind_FOUND ${Valgrind_FOUND} CACHE BOOL "Flag whether Valgrind package was found")
  mark_as_advanced(Valgrind_FOUND Valgrind_EXECUTABLE)

endif()
