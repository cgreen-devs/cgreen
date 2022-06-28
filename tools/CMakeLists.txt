include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})

set(RUNNER_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/cgreen-runner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/gopt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/gopt-errors.c
  ${CMAKE_CURRENT_SOURCE_DIR}/runner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/discoverer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test_item.c
  ${CMAKE_CURRENT_SOURCE_DIR}/io.c
)
set_source_files_properties(${RUNNER_SRCS} PROPERTIES LANGUAGE C)

include(DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${RUNNER_SRCS}")

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
