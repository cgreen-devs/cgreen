include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

remove_definitions(-std=c++0x -Weffc++)

add_executable(cgreen-runner cgreen-runner.c gopt.c xml_reporter.c runner.c)
target_link_libraries(cgreen-runner ${CGREEN_SHARED_LIBRARY} dl m)

install(TARGETS cgreen-runner DESTINATION ${BIN_INSTALL_DIR})

set(CGREEN_RUNNER_TESTS_LIBRARY
        cgreen_runner_tests
        CACHE INTERNAL "cgreen-runner tests shared library"
)
set(runner_tests_SRCS
        runnerTests.c)

add_library(${CGREEN_RUNNER_TESTS_LIBRARY} SHARED ${runner_tests_SRCS})

target_link_libraries(${CGREEN_RUNNER_TESTS_LIBRARY} ${CGREEN_SHARED_LIBRARY} dl m)

add_test(NAME cgreen_runner_unit_tests COMMAND cgreen-runner $<TARGET_FILE_NAME:cgreen_runner_tests>)

