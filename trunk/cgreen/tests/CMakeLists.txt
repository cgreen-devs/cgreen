if (WITH_CXX)
  project(tests CXX)
else()
  project(tests C)
endif (WITH_CXX)

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
  reflective_tests.c
  reflective_runner_no_teardown_tests.c
  slurp_test.c
  unit_tests.c
  vector_tests.c
)

set(CGREEN_TEST_LIBRARY
  cgreen_tests
  CACHE INTERNAL "cgreen tests shared library"
)

if (WITH_CXX)
  set(test_SRCS ${test_SRCS} cpp_assertion_tests.cpp)
  SET_SOURCE_FILES_PROPERTIES( ${test_SRCS} PROPERTIES LANGUAGE CXX )
endif (WITH_CXX)

add_library(${CGREEN_TEST_LIBRARY} SHARED ${test_SRCS})

if (WITH_STATIC_LIBRARY)
  set(CGREEN_LIBRARY ${CGREEN_STATIC_LIBRARY})
else ()
  set(CGREEN_LIBRARY ${CGREEN_STATIC_LIBRARY})
endif()

if (WITH_CXX)
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_SHARED_LIBRARY} m stdc++)
else()
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_SHARED_LIBRARY} m)
endif (WITH_CXX)

if (WITH_CXX)
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY} m stdc++)
else()
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY} m)
endif(WITH_CXX)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/some_file "Some stuff")

macro_add_unit_test(test_cgreen "${test_SRCS}" "${TEST_TARGET_LIBRARIES}")

add_test(test_cgreen_named ${RUNTIME_OUTPUT_PATH}/test_cgreen integer_one_should_assert_true)

