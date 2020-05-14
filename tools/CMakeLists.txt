include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})

set(RUNNER_SRCS
    cgreen-runner.c gopt.c gopt-errors.c runner.c discoverer.c test_item.c io.c)
set_source_files_properties(${RUNNER_SRCS} PROPERTIES LANGUAGE C)

add_executable(cgreen-runner ${RUNNER_SRCS})
target_link_libraries(cgreen-runner ${CGREEN_SHARED_LIBRARY} ${CMAKE_DL_LIBS})

install(TARGETS cgreen-runner
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(PROGRAMS cgreen-debug
  DESTINATION ${CMAKE_INSTALL_BINDIR})
