project(cgreen-runner C)

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})

remove_definitions(-Weffc++)
add_definitions(-std=gnu99)
set(RUNNER_SRCS
    cgreen-runner.c gopt.c xml_reporter.c runner.c)

add_executable(cgreen-runner ${RUNNER_SRCS})
target_link_libraries(cgreen-runner ${CGREEN_SHARED_LIBRARY} ${CMAKE_DL_LIBS})

install(TARGETS cgreen-runner 
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  DESTINATION ${BIN_INSTALL_DIR}
)
