# Generate gitrevision.h if Git is available and the .git directory is found.
# NOTE: $GITDIR is determined in top-level CMakeLists.txt
if (GITDIR AND GIT_EXECUTABLE)
  # Create gitrevision.h
  add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E echo_append "#define GITREVISION \"" > ${PROJECT_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND ${GIT_EXECUTABLE} describe --tags --always --abbrev=7 --dirty=-modified >> ${PROJECT_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND ${CMAKE_COMMAND} -E echo_append "\"" >> ${PROJECT_SOURCE_DIR}/gitrevision.h.tmp
    COMMAND sed -e N -e "s/\\n//g" ${PROJECT_SOURCE_DIR}/gitrevision.h.tmp > ${PROJECT_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_SOURCE_DIR}/gitrevision.h.tmp
    DEPENDS ${GITDIR}
    VERBATIM
    COMMENT "-- Generating gitrevision.h"
    )
else()
  # No version control
  # e.g. when the software is built from a source tarball
  message(WARNING "-- Unable to find git. Setting git revision to 'unknown'.")
  add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/gitrevision.h
    COMMAND ${CMAKE_COMMAND} -E echo "#define GITREVISION \"unknown\"" > ${PROJECT_SOURCE_DIR}/gitrevision.h
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
  ${CMAKE_CURRENT_SOURCE_DIR}/assertions.c
  ${CMAKE_CURRENT_SOURCE_DIR}/boxed_double.c
  ${CMAKE_CURRENT_SOURCE_DIR}/breadcrumb.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cgreen_time.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cgreen_value.c
  ${CMAKE_CURRENT_SOURCE_DIR}/constraint.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cute_reporter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cdash_reporter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/messaging.c
  ${CMAKE_CURRENT_SOURCE_DIR}/message_formatting.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mocks.c
  ${CMAKE_CURRENT_SOURCE_DIR}/parameters.c
  ${CMAKE_CURRENT_SOURCE_DIR}/reporter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/runner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/string_comparison.c
  ${CMAKE_CURRENT_SOURCE_DIR}/suite.c
  ${CMAKE_CURRENT_SOURCE_DIR}/text_reporter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xml_reporter.c
)

if (MSYS)
  # Msys2 is difficult since it really is three different "OS":es, Msys native, W32 and W64
  # To get somewhere, let's use the native Msys2, which actually is Cygwin/UNIX.
  LIST(APPEND cgreen_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_cgreen_pipe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_cgreen_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_runner_platform.c
  )
elseif (UNIX OR CYGWIN)
  LIST(APPEND cgreen_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_cgreen_pipe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_cgreen_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/posix_runner_platform.c
  )
elseif(WIN32)
 LIST(APPEND cgreen_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/win32_cgreen_pipe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/win32_cgreen_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/win32_runner_platform.c
  )
else()
  message(FATAL_ERROR "Cgreen can currently only be compiled for Msys2, Cygwin, MacOSX and Windows.")
endif()
SET_SOURCE_FILES_PROPERTIES(${cgreen_SRCS} PROPERTIES LANGUAGE C)

set(cgreen_SRCS ${cgreen_SRCS}
  ${CMAKE_CURRENT_SOURCE_DIR}/cpp_assertions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cpp_constraint.cpp
)
SET_SOURCE_FILES_PROPERTIES(cpp_assertions.cpp cpp_constraint.cpp PROPERTIES LANGUAGE CXX )


include(DefineRelativeFilePaths)
cmake_define_relative_file_paths ("${cgreen_SRCS}")

include_directories(
  ${CGREEN_PUBLIC_INCLUDE_DIRS}
  ${CGREEN_PRIVATE_INCLUDE_DIRS}
)

### cgreen
set(CGREEN_LIBRARY_NAME cgreen)
add_library(${CGREEN_SHARED_LIBRARY} SHARED ${cgreen_SRCS} ${PROJECT_SOURCE_DIR}/gitrevision.h)

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
