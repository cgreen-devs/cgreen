if(CGREEN_BUILD_RUNNER)
  find_package(LibBfd)

  if(LibBfd_FOUND)
    include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} ${LIBBFD_INCLUDE_DIRS})

    # Need to explicitly add libintl - Cygwin issue with binutils-2.37-2
    if (CYGWIN)
      find_library(EXPLICIT_INTLIB_ONLY_REQUIRED_FOR_CYGWIN NAMES intl libintl)
    else ()
      set(EXPLICIT_INTLIB_ONLY_REQUIRED_FOR_CYGWIN "")
    endif()

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
      ${EXPLICIT_INTLIB_ONLY_REQUIRED_FOR_CYGWIN}
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
    message(WARNING "libbfd not found, cgreen-runner will not be built")
  endif()
endif()
