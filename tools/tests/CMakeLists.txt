if (WITH_CXX)
  project(cgreen-runner-tests CXX)
else()
  project(cgreen-runner-tests C)
endif (WITH_CXX)

include_directories(.. ${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

set(CGREEN_RUNNER_TESTS_LIBRARY
  cgreen_runner_tests
  CACHE INTERNAL "cgreen-runner tests shared library"
)

set(RUNNER_TESTS_SRCS
  runnerTests.c)

if (WITH_CXX)
   set_source_files_properties( ${RUNNER_TESTS_SRCS} PROPERTIES LANGUAGE CXX)
else()
   remove_definitions(-Weffc++)
   add_definitions(-std=gnu99)
endif()

add_library(${CGREEN_RUNNER_TESTS_LIBRARY} SHARED ${RUNNER_TESTS_SRCS})

target_link_libraries(${CGREEN_RUNNER_TESTS_LIBRARY} ${CGREEN_SHARED_LIBRARY} ${CMAKE_DL_LIBS} m)

add_test(NAME cgreen_runner_unit_tests COMMAND cgreen-runner $<TARGET_FILE_DIR:cgreen_runner_tests>/$<TARGET_FILE_NAME:cgreen_runner_tests>)
