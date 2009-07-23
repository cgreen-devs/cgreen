project(tests C)

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

set(test_SRCS
  all_tests.c
  assertion_tests.c
  breadcrumb_tests.c
  collector_tests.c
  constraint_tests.c
  cute_reporter_tests.c
  messaging_tests.c
  mocks_tests.c
  parameters_test.c
  slurp_test.c
  unit_tests.c
  vector_tests.c
)

set(TEST_TARGET_LIBRARIES ${CGREEN_SHARED_LIBRARY} m)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/some_file "Some stuff")
if (WIN32)
  file(WRITE ${CMAKE_BINARY_DIR}/bin/tests/some_file "Some stuff")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
endif (WIN32)
macro_add_unit_test(test_cgreen "${test_SRCS}" "${TEST_TARGET_LIBRARIES}")

