# Generate gitrevision.h if Git is available
# and the .git directory is present
find_program(GIT_SCM git DOC "Git version control")
mark_as_advanced(GIT_SCM)
find_file(GITDIR NAMES .git PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
if (GIT_SCM AND GITDIR)
  # Create gitrevision.h
  # that depends on the Git HEAD log
  add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E echo_append "#define GITREVISION \"" > ${CMAKE_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND ${GIT_SCM} describe --tags --always --abbrev=7 --dirty=-modified >> ${CMAKE_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND ${CMAKE_COMMAND} -E echo_append "\"" >> ${CMAKE_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND sed -e N -e "s/\\n//g" ${CMAKE_SOURCE_DIR}/gitrevision.h.tmp > ${CMAKE_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/gitrevision.h.tmp
    DEPENDS ${GITDIR}/logs/HEAD
    VERBATIM
    )
else()
  # No version control
  # e.g. when the software is built from a source tarball
  add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E echo "#define GITREVISION \"unknown\"" > ${CMAKE_SOURCE_DIR}/gitrevision.h
    VERBATIM
    )
endif()


set(CGREEN_PUBLIC_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  CACHE INTERNAL "cgreen public include directories"
)

set(CGREEN_PRIVATE_INCLUDE_DIRS
  ${PROJECT_BINARY_DIR}
)

if (MSVC)
  LIST(APPEND CGREEN_PRIVATE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/../include/cgreen/internal/windows_headers
  )
endif(MSVC)

set(CGREEN_SHARED_LIBRARY
  cgreen_shared
  CACHE INTERNAL "cgreen shared library"
)

if (WIN32 AND NOT CYGWIN)
  set(CGREEN_LINK_LIBRARIES
    winmm.lib
  )
endif(WIN32 AND NOT CYGWIN)

set(cgreen_SRCS
  assertions.c
  boxed_double.c
  breadcrumb.c
  cgreen_time.c
  cgreen_value.c
  constraint.c
  constraint_syntax_helpers.c
  cute_reporter.c
  cdash_reporter.c
  messaging.c
  message_formatting.c
  mocks.c
  parameters.c
  reporter.c
  runner.c
  string_comparison.c
  suite.c
  text_reporter.c
  utils.c
  vector.c
  xml_reporter.c
)

if (MSYS)
  # Msys2 is difficult since it really is three different "OS":es, Msys native, W32 and W64
  # To get somewhere, let's use the native Msys2, which actually is Cygwin/UNIX.
  LIST(APPEND cgreen_SRCS
    posix_cgreen_pipe.c
    posix_cgreen_time.c
    posix_runner_platform.c
  )
elseif (UNIX OR CYGWIN)
  LIST(APPEND cgreen_SRCS
    posix_cgreen_pipe.c
    posix_cgreen_time.c
    posix_runner_platform.c
  )
elseif(WIN32)
 LIST(APPEND cgreen_SRCS
    win32_cgreen_pipe.c
    win32_cgreen_time.c
    win32_runner_platform.c
  )
else()
  message(FATAL_ERROR "Cgreen can currently only be compiled for Msys2, Cygwin, MacOSX and Windows.")
endif()
SET_SOURCE_FILES_PROPERTIES(${cgreen_SRCS} PROPERTIES LANGUAGE C)

set(cgreen_SRCS ${cgreen_SRCS}
  cpp_assertions.cpp
  cpp_constraint.cpp
)
SET_SOURCE_FILES_PROPERTIES(cpp_assertions.cpp cpp_constraint.cpp PROPERTIES LANGUAGE CXX )

include_directories(
  ${CGREEN_PUBLIC_INCLUDE_DIRS}
  ${CGREEN_PRIVATE_INCLUDE_DIRS}
)

### cgreen
set(CGREEN_LIBRARY_NAME cgreen)
add_library(${CGREEN_SHARED_LIBRARY} SHARED ${cgreen_SRCS} ${CMAKE_SOURCE_DIR}/gitrevision.h)

target_link_libraries(
  ${CGREEN_SHARED_LIBRARY}
  ${CGREEN_LINK_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIB}
  ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}
)

set_target_properties(
  ${CGREEN_SHARED_LIBRARY}
    PROPERTIES
      VERSION
        ${LIBRARY_VERSION}
      SOVERSION
        ${LIBRARY_SOVERSION}
      OUTPUT_NAME
        ${CGREEN_LIBRARY_NAME}
)

install(
  TARGETS ${CGREEN_SHARED_LIBRARY}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT libraries
)

if (CGREEN_WITH_STATIC_LIBRARY)

  set(CGREEN_STATIC_LIBRARY
    cgreen_static
    CACHE INTERNAL "cgreen static library"
  )

  add_library(${CGREEN_STATIC_LIBRARY} STATIC ${cgreen_SRCS})

  target_link_libraries(${CGREEN_STATIC_LIBRARY}
    ${CGREEN_LINK_LIBRARIES}
    ${MATH_LIB}
    ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}
  )

  set_target_properties(
    ${CGREEN_STATIC_LIBRARY}
      PROPERTIES
        VERSION
          ${LIBRARY_VERSION}
        SOVERSION
          ${LIBRARY_SOVERSION}
        OUTPUT_NAME
          ${CGREEN_LIBRARY_NAME}
  )

  install(
    TARGETS ${CGREEN_STATIC_LIBRARY}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT libraries
  )
endif(CGREEN_WITH_STATIC_LIBRARY)
