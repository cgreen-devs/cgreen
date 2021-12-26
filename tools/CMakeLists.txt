include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})

find_package(LibBfd)

if(LibBfd_FOUND)
  set(RUNNER_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/cgreen-runner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gopt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gopt-errors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/runner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/discoverer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_item.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bfd_adapter.c
  )
  set_source_files_properties(${RUNNER_SRCS} PROPERTIES LANGUAGE C)

  include(DefineRelativeFilePaths)
  cmake_define_relative_file_paths ("${RUNNER_SRCS}")

  add_executable(cgreen-runner ${RUNNER_SRCS})
  target_link_libraries(cgreen-runner PRIVATE
    ${CGREEN_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${LIBBFD_LIBRARIES}
  )

  install(TARGETS cgreen-runner
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(PROGRAMS cgreen-debug
    DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
  message(WARNING “libbfd not found, cgreen-runner will not be build”)
endif()
