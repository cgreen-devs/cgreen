# This subdirectory verifies that the user level api compiles using
# only the publiced includes by compiling test files refering to that
# api. Goal is to reference, and thus verify, the whole api so that
# unintended changes to the API gets noticed early.
#
# There is no need to run the tests...

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS})

set(api_SRCS
  core_api.c
  suites_without_context_api.c
  suites_with_context_api.c
)

set(CGREEN_API_TEST_LIBRARY
  cgreen_api
  CACHE INTERNAL "cgreen library of user level api verification source"
)

add_library(${CGREEN_API_TEST_LIBRARY} SHARED ${api_SRCS})

target_link_libraries(${CGREEN_API_TEST_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

#macro_add_test(NAME api_compilation_test
#              COMMAND ${PROJECT_SOURCE_DIR}/tools/compare_test_output_to_expected
#                      ${PROJECT_BINARY_DIR}/tools                       # Where is cgreen-runner?
#                      $<TARGET_FILE_NAME:${CGREEN_API_TEST_LIBRARY}>  # Library to run
#                      ${PROJECT_SOURCE_DIR}                             # Remove lines with these from output
#                      ${CGREEN_API_TEST_LIBRARY}                      # Normalized library name
#                      ${CMAKE_CURRENT_SOURCE_DIR})                    # Where is the expected output?
