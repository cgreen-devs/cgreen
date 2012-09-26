include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

remove_definitions(-std=c++0x -Weffc++)
add_definitions(-std=c99)
add_executable(cgreen-runner cgreen-runner.c gopt.c xml_reporter.c)
target_link_libraries(cgreen-runner ${CGREEN_SHARED_LIBRARY} dl m)

install(TARGETS cgreen-runner DESTINATION ${BIN_INSTALL_DIR})
