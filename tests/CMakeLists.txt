set(CGREEN_TEST_INCLUDE_DIRS
  ${CGREEN_PUBLIC_INCLUDE_DIRS} 
  ${CMAKE_CURRENT_BINARY_DIR}
)


if (MSVC)
  LIST(APPEND CGREEN_TEST_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include/cgreen/internal/windows_headers
  )
endif(MSVC)


include_directories(${CGREEN_TEST_INCLUDE_DIRS})

# Dependency on our own library
if (CGREEN_WITH_STATIC_LIBRARY)
  set(CGREEN_LIBRARY ${CGREEN_STATIC_LIBRARY})
else ()
  set(CGREEN_LIBRARY ${CGREEN_SHARED_LIBRARY})
endif()


# C tests, library to use with runner, and a main program
set(c_tests_library_SRCS
  assertion_tests.c
  breadcrumb_tests.c
  constraint_tests.c
  cute_reporter_tests.c
  cdash_reporter_tests.c
  messaging_tests.c
  message_formatting_tests.c
  mocks_tests.c
  parameters_test.c
  reflective_tests.c
  reflective_runner_no_teardown_tests.c
  unit_tests.c
  vector_tests.c
)
SET_SOURCE_FILES_PROPERTIES(${c_tests_library_SRCS} PROPERTIES LANGUAGE C)

set(CGREEN_C_TESTS_LIBRARY
  cgreen_c_tests
  CACHE INTERNAL "cgreen shared library with tests for C"
)


add_library(${CGREEN_C_TESTS_LIBRARY} SHARED ${c_tests_library_SRCS})
target_link_libraries(${CGREEN_C_TESTS_LIBRARY} ${CGREEN_LIBRARY})

set(c_tests_SRCS
  all_c_tests.c
  ${c_tests_library_SRCS}
)
SET_SOURCE_FILES_PROPERTIES(${c_tests_SRCS} PROPERTIES LANGUAGE C)

set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY})

macro_add_unit_test(test_cgreen_c "${c_tests_SRCS}" "${TEST_TARGET_LIBRARIES}")
macro_add_test(NAME test_cgreen_run_named_test COMMAND test_cgreen_c integer_one_should_assert_true)
macro_add_test(NAME runner_test_cgreen_c COMMAND cgreen-runner -x TEST ${CMAKE_SHARED_LIBRARY_PREFIX}${CGREEN_C_TESTS_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX})


# C++ tests, library to use with runner, and a main program
set(cpp_tests_library_SRCS
  cpp_assertion_tests.cpp
  breadcrumb_tests.cpp
  constraint_tests.cpp
  cute_reporter_tests.cpp
  cdash_reporter_tests.cpp
  messaging_tests.cpp
  mocks_tests.cpp
  parameters_test.cpp
  reflective_tests.cpp
  reflective_runner_no_teardown_tests.cpp
  unit_tests.cpp
  vector_tests.cpp
)
SET_SOURCE_FILES_PROPERTIES(${cpp_tests_library_SRCS} PROPERTIES LANGUAGE CXX)

set(CGREEN_CPP_TESTS_LIBRARY
  cgreen_cpp_tests
  CACHE INTERNAL "cgreen shared library with tests for C++"
)
add_library(${CGREEN_CPP_TESTS_LIBRARY} SHARED ${cpp_tests_library_SRCS})
target_link_libraries(${CGREEN_CPP_TESTS_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})

set(cpp_tests_SRCS
  all_cpp_tests.cpp
  ${cpp_tests_library_SRCS}
)
SET_SOURCE_FILES_PROPERTIES(${cpp_tests_SRCS} PROPERTIES LANGUAGE CXX)

macro_add_unit_test(test_cgreen_cpp "${cpp_tests_SRCS}" "${TEST_TARGET_LIBRARIES}" ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
macro_add_test(NAME runner_test_cgreen_cpp COMMAND cgreen-runner -x TEST ${CMAKE_SHARED_LIBRARY_PREFIX}${CGREEN_CPP_TESTS_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX})



# Libraries for a set of output comparing tests to run with runner

set(constraint_messages_library constraint_messages_tests)
set(constraint_messages_library_SRCS constraint_messages_tests.c)
add_library(${constraint_messages_library} SHARED ${constraint_messages_library_SRCS})
target_link_libraries(${constraint_messages_library} ${CGREEN_LIBRARY})

set(mock_messages_library mock_messages_tests)
set(mock_messages_library_SRCS mock_messages_tests.c)
add_library(${mock_messages_library} SHARED ${mock_messages_library_SRCS})
target_link_libraries(${mock_messages_library} ${CGREEN_LIBRARY})

set(failure_messages_library failure_messages_tests)
set(failure_messages_library_SRCS failure_messages_tests.c)
add_library(${failure_messages_library} SHARED ${failure_messages_library_SRCS})
target_link_libraries(${failure_messages_library} ${CGREEN_LIBRARY})

set(assertion_messages_library assertion_messages_tests)
set(assertion_messages_library_SRCS assertion_messages_tests.c)
add_library(${assertion_messages_library} SHARED ${assertion_messages_library_SRCS})
target_link_libraries(${assertion_messages_library} ${CGREEN_LIBRARY})

set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY})

macro_add_test(
    NAME constraint_messsages
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../tools/cgreen_runner_output_diff
            constraint_messages_tests       # Name
            ${CMAKE_CURRENT_SOURCE_DIR}     # Where sources are
            ${constraint_messages_library}.${LANG}.expected
)

macro_add_test(
    NAME mock_messsages
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../tools/cgreen_runner_output_diff
            mock_messages_tests             # Name
            ${CMAKE_CURRENT_SOURCE_DIR}     # Where sources are
            ${mock_messages_library}.${LANG}.expected
)

macro_add_test(NAME failure_messages
    COMMAND env "CGREEN_PER_TEST_TIMEOUT=2" ${CMAKE_CURRENT_SOURCE_DIR}/../tools/cgreen_runner_output_diff
            failure_messages_tests          # Name
            ${CMAKE_CURRENT_SOURCE_DIR}     # Where sources are
            ${failure_messages_library}.${LANG}.expected
)

macro_add_test(NAME assertion_messages
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../tools/cgreen_runner_output_diff
            assertion_messages_tests        # Name
            ${CMAKE_CURRENT_SOURCE_DIR}     # Where sources are
            ${assertion_messages_library}.${LANG}.expected
)

# add verification that all public api is available as it should
add_subdirectory(api)
