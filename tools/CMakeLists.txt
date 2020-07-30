include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} ${CURRENT_BINARY_DIR})

set(RUNNER_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/cgreen-runner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/gopt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/gopt-errors.c
  ${CMAKE_CURRENT_SOURCE_DIR}/runner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/discoverer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test_item.c
  ${CMAKE_CURRENT_SOURCE_DIR}/io.c
)
set_source_files_properties(${RUNNER_SRCS} PROPERTIES LANGUAGE C)

# Do we have an "nm"?
include(FindNm)

if (NM_FOUND)
   set_source_files_properties(discoverer.c PROPERTIES COMPILE_FLAGS -DNM_EXECUTABLE='\"${NM_EXECUTABLE}\"')

   include(DefineRelativeFilePaths)
   cmake_define_relative_file_paths("${RUNNER_SRCS}")

   add_executable(cgreen-runner ${RUNNER_SRCS})
   target_link_libraries(cgreen-runner ${CGREEN_LIBRARY} ${CMAKE_DL_LIBS})

   install(TARGETS cgreen-runner
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     DESTINATION ${CMAKE_INSTALL_BINDIR}
   )

   install(PROGRAMS cgreen-debug
     DESTINATION ${CMAKE_INSTALL_BINDIR})

   set(CGREEN_RUNNER_TESTS_LIBRARY
     cgreen_runner_tests
     CACHE INTERNAL "cgreen-runner tests shared library"
   )
   set(RUNNER_TESTS_SRCS
     runner_unit_tests.c
     test_item.c
   )
   add_library(${CGREEN_RUNNER_TESTS_LIBRARY} SHARED ${RUNNER_TESTS_SRCS})
   target_link_libraries(${CGREEN_RUNNER_TESTS_LIBRARY} ${CGREEN_LIBRARY})

   # Due to some (of many) CMake irregularities to reference the test libraries
   # we can't just use its CMake name variable, but have to look it up with
   # some special attributes of the library:
   #     $<TARGET_FILE_DIR:the_lib>/$<TARGET_FILE_NAME:the_lib>
   #
   SET(CGREEN_RUNNER_TESTS_LIBRARY "$<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests>")

   macro_add_test(NAME cgreen_runner_unit_tests
     COMMAND cgreen-runner ${CGREEN_RUNNER_TESTS_LIBRARY})

   macro_add_test(NAME cgreen_runner_usage
     COMMAND cgreen-runner --help)

   macro_add_test(NAME cgreen_runner_quiet
     COMMAND cgreen-runner -q ${CGREEN_RUNNER_TESTS_LIBRARY})

   macro_add_test(NAME cgreen_runner_verbose
     COMMAND cgreen-runner -v -C ${CGREEN_RUNNER_TESTS_LIBRARY})

   macro_add_test(NAME cgreen_runner_version
     COMMAND cgreen-runner --version)

   macro_add_test(NAME cgreen_runner_single_explicit_named_test
     COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests> Runner:can_match_test_name)

   macro_add_test(NAME cgreen_runner_patternmatched_testnames
     COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests> Runner:can*)

   macro_add_test(NAME cgreen_runner_suite_name
       COMMAND cgreen-runner -s Suite ${CGREEN_RUNNER_TESTS_LIBRARY})

   macro_add_test(NAME cgreen_runner_fail_on_non_existing_library
       COMMAND cgreen-runner Suite non_existent_library)
   set_tests_properties(cgreen_runner_fail_on_non_existing_library PROPERTIES WILL_FAIL true)

   macro_add_test(NAME cgreen_runner_fail_on_non_existing_library_with_suite
       COMMAND cgreen-runner -s Suite non_existent_library
       WILL_FAIL)
   set_tests_properties(cgreen_runner_fail_on_non_existing_library_with_suite PROPERTIES WILL_FAIL true)

   macro_add_test(NAME cgreen_runner_patternmatched_testnames_in_patternmatched_context
     COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests> Run*:can*)

   macro_add_test(NAME cgreen_runner_wildcarded_tests_in_named_context
     COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests> Runner:*)

   macro_add_test(NAME cgreen_runner_wildcarded_tests_in_wildcarded_context
     COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests> *:*)

   macro_add_test(NAME cgreen_runner_with_xml_reporter
     COMMAND cgreen-runner --xml TEST --suite cgreen_runner_tests $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests>)

   if (CGREEN_WITH_LIBXML2)
      macro_add_test(NAME cgreen_runner_with_libxml2_reporter
         COMMAND cgreen-runner --libxml2 TEST --suite cgreen_runner_tests $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests>)
   endif (CGREEN_WITH_LIBXML2)

   macro_add_test(NAME cgreen_runner_multiple_libraries
     COMMAND cgreen-runner ${CGREEN_RUNNER_TESTS_LIBRARY} ${CGREEN_RUNNER_TESTS_LIBRARY} ${CGREEN_RUNNER_TESTS_LIBRARY})

else()
   message("No 'nm' found on this system. 'cgreen-runner' will not be built.")
endif()
