
if (WITH_CXX)
  project(tests CXX)
else()
  project(tests C)
endif (WITH_CXX)

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

set(test_SRCS
  all_tests.c
  assertion_tests.c
  cpp_assertion_tests.cpp
  breadcrumb_tests.c
  collector_tests.c
  constraint_tests.c
  cute_reporter_tests.c
  messaging_tests.c
  mocks_tests.c
  parameters_test.c
  reflective_tests.c
  slurp_test.c
  unit_tests.c
  vector_tests.c
)

set(CGREEN_TEST_LIBRARY
  cgreen_tests
  CACHE INTERNAL "cgreen tests shared library"
)

add_library(${CGREEN_TEST_LIBRARY} SHARED ${test_SRCS})
target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_STATIC_LIBRARY} m)

if (WITH_CXX)
  SET_SOURCE_FILES_PROPERTIES( ${test_SRCS} PROPERTIES LANGUAGE CXX )
endif (WITH_CXX)

set(TEST_TARGET_LIBRARIES ${CGREEN_STATIC_LIBRARY} m)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/some_file "Some stuff")
if (WIN32)
  file(WRITE ${CMAKE_BINARY_DIR}/bin/tests/some_file "Some stuff")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)
endif (WIN32)

macro_add_unit_test(test_cgreen "${test_SRCS}" "${TEST_TARGET_LIBRARIES}")

add_test(test_cgreen_named test_cgreen integer_one_should_assert_true)
