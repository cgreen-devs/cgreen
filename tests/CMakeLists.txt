if (WITH_CXX)
  project(tests CXX)
else()
  project(tests C)
endif (WITH_CXX)

set(CGREEN_TEST_INCLUDE_DIRS
  ${CGREEN_PUBLIC_INCLUDE_DIRS} 
  ${CMAKE_BINARY_DIR}
)


if (MSVC)
  LIST(APPEND CGREEN_TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include/cgreen/internal/windows_headers
  )
endif(MSVC)


include_directories(${CGREEN_TEST_INCLUDE_DIRS})

set(test_library_SRCS
  assertion_tests.c
  breadcrumb_tests.c
  constraint_tests.c
  cute_reporter_tests.c
  messaging_tests.c
  message_formatting_tests.c
  mocks_tests.c
  parameters_test.c
  reflective_tests.c
  reflective_runner_no_teardown_tests.c
  unit_tests.c
  vector_tests.c
)

if (WITH_CXX)
  set(test_library_SRCS ${test_library_SRCS} cpp_assertion_tests.cpp)
endif(WITH_CXX)

set(test_SRCS
  all_tests.c
  ${test_library_SRCS}
)

set(CGREEN_TEST_LIBRARY
  cgreen_tests
  CACHE INTERNAL "cgreen tests shared library"
)

set(all_constraints_library_SRCS
  all_constraints_printout.c)

set(all_constraints_SRCS
  all_constraints_tests.c
  ${all_constraints_library_SRCS})

if (WITH_CXX)
  SET_SOURCE_FILES_PROPERTIES( ${test_SRCS} PROPERTIES LANGUAGE CXX )
  SET_SOURCE_FILES_PROPERTIES( ${all_constraints_SRCS} PROPERTIES LANGUAGE CXX )
endif (WITH_CXX)

add_library(${CGREEN_TEST_LIBRARY} SHARED ${test_library_SRCS})

add_library(cgreen_all_constraints_library SHARED ${all_constraints_library_SRCS})

add_executable(test_all_constraints ${all_constraints_SRCS})

if (WITH_STATIC_LIBRARY)
  set(CGREEN_LIBRARY ${CGREEN_STATIC_LIBRARY})
else ()
  set(CGREEN_LIBRARY ${CGREEN_SHARED_LIBRARY})
endif()

if (WITH_CXX)
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(test_all_constraints ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(cgreen_all_constraints_library ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
else()
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_LIBRARY})
  target_link_libraries(test_all_constraints ${CGREEN_LIBRARY})
  target_link_libraries(cgreen_all_constraints_library ${CGREEN_LIBRARY})
endif (WITH_CXX)

if (WITH_CXX)
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
else()
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY})
endif(WITH_CXX)

macro_add_unit_test(test_cgreen "${test_SRCS}" "${TEST_TARGET_LIBRARIES}")

macro_add_test(NAME test_cgreen_run_named_test COMMAND test_cgreen integer_one_should_assert_true)

# add verification that all public api is available as it should
add_subdirectory(api)
