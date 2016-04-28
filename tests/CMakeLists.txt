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

set(test_library_SRCS
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
  cpp_assertion_tests.cpp
)

SET_SOURCE_FILES_PROPERTIES(cpp_assertions.cpp cpp_asssertion_tests.cpp PROPERTIES LANGUAGE CXX )

set(test_SRCS
  all_tests.c
  ${test_library_SRCS}
)

set(CGREEN_TEST_LIBRARY
  cgreen_tests
  CACHE INTERNAL "cgreen tests shared library"
)

set(constraint_messages_library
  constraint_messages_tests
)

set(constraint_messages_library_SRCS
  constraint_messages_tests.c)

set(mock_messages_library
    mock_messages_tests
)

set(mock_messages_library_SRCS
    mock_messages_tests.c)

set(failure_messages_library
  failure_messages_tests
)

set(failure_messages_library_SRCS
  failure_messages_tests.c)

set(assertion_messages_library
  assertion_messages_tests
)

set(assertion_messages_library_SRCS
  assertion_messages_tests.c)

if (CGREEN_WITH_CXX)
  SET_SOURCE_FILES_PROPERTIES( ${test_SRCS} PROPERTIES LANGUAGE CXX )
  SET_SOURCE_FILES_PROPERTIES( ${constraint_messages_library_SRCS} PROPERTIES LANGUAGE CXX )
  SET_SOURCE_FILES_PROPERTIES( ${mock_messages_library_SRCS} PROPERTIES LANGUAGE CXX )
  SET_SOURCE_FILES_PROPERTIES( ${failure_messages_library_SRCS} PROPERTIES LANGUAGE CXX )
  SET_SOURCE_FILES_PROPERTIES( ${assertion_messages_library_SRCS} PROPERTIES LANGUAGE CXX )
endif (CGREEN_WITH_CXX)

add_library(${CGREEN_TEST_LIBRARY} SHARED ${test_library_SRCS})
add_library(${constraint_messages_library} SHARED ${constraint_messages_library_SRCS})
add_library(${mock_messages_library} SHARED ${mock_messages_library_SRCS})
add_library(${failure_messages_library} SHARED ${failure_messages_library_SRCS})
add_library(${assertion_messages_library} SHARED ${assertion_messages_library_SRCS})

if (CGREEN_WITH_STATIC_LIBRARY)
  set(CGREEN_LIBRARY ${CGREEN_STATIC_LIBRARY})
else ()
  set(CGREEN_LIBRARY ${CGREEN_SHARED_LIBRARY})
endif()

if (CGREEN_WITH_CXX)
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(${constraint_messages_library} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(${mock_messages_library} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(${failure_messages_library} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  target_link_libraries(${assertion_messages_library} ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
else()
  target_link_libraries(${CGREEN_TEST_LIBRARY} ${CGREEN_LIBRARY})
  target_link_libraries(${constraint_messages_library} ${CGREEN_LIBRARY})
  target_link_libraries(${mock_messages_library} ${CGREEN_LIBRARY})
  target_link_libraries(${failure_messages_library} ${CGREEN_LIBRARY})
  target_link_libraries(${assertion_messages_library} ${CGREEN_LIBRARY})
endif (CGREEN_WITH_CXX)

if (CGREEN_WITH_CXX)
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
else()
  set(TEST_TARGET_LIBRARIES ${CGREEN_LIBRARY})
endif(CGREEN_WITH_CXX)

macro_add_unit_test(test_cgreen "${test_SRCS}" "${TEST_TARGET_LIBRARIES}")
macro_add_test(NAME test_cgreen_run_named_test COMMAND test_cgreen integer_one_should_assert_true)

if (CGREEN_WITH_CXX)
  set(LANG c++)
else()
  set(LANG c)
endif()

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
