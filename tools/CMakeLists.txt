include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable(cgreen-runner cgreen-runner.c)
target_link_libraries(cgreen-runner ${CGREEN_STATIC_LIBRARY} dl m)
