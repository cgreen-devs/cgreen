include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

remove_definitions(-std=c++0x -Weffc++)
add_executable(cgreen-runner cgreen-runner.c)
target_link_libraries(cgreen-runner ${CGREEN_SHARED_LIBRARY} dl m)

install(TARGETS cgreen-runner DESTINATION ${BIN_INSTALL_DIR})
