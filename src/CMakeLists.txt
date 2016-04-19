if (UNIX)
  find_library(MATH_LIB m)
endif(UNIX)

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
)

if (UNIX)
  LIST(APPEND cgreen_SRCS 
    posix_cgreen_pipe.c
    posix_cgreen_time.c
    posix_runner_platform.c
  )
else(UNIX)
  if(WIN32)
    LIST(APPEND cgreen_SRCS 
      win32_cgreen_pipe.c
      win32_runner_platform.c
    )
  else(WIN32)
    message(FATAL_ERROR "cgreen currently only works on UNIX, Cygwin, MacOSX and Windows. Please file a bug.")
  endif(WIN32)
endif(UNIX)

if (CGREEN_WITH_CXX)
  set(CGREEN_LIBRARY_NAME cgreen++)
  set(cgreen_SRCS ${cgreen_SRCS} 
    cpp_assertions.cpp
    cpp_constraint.cpp
  )
  SET_SOURCE_FILES_PROPERTIES( ${cgreen_SRCS} PROPERTIES LANGUAGE CXX )
else (CGREEN_WITH_CXX)
  set(CGREEN_LIBRARY_NAME cgreen)
endif (CGREEN_WITH_CXX)
 
include_directories(
  ${CGREEN_PUBLIC_INCLUDE_DIRS}
  ${CGREEN_PRIVATE_INCLUDE_DIRS}
)

### cgreen
add_library(${CGREEN_SHARED_LIBRARY} SHARED ${cgreen_SRCS})

if (CGREEN_WITH_CXX)
    target_link_libraries(
      ${CGREEN_SHARED_LIBRARY} 
      ${CGREEN_LINK_LIBRARIES} 
      ${CMAKE_THREAD_LIBS_INIT}
      ${MATH_LIB} 
      ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}
    )
else (CGREEN_WITH_CXX)
    target_link_libraries(
      ${CGREEN_SHARED_LIBRARY}
      ${CGREEN_LINK_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      ${MATH_LIB} 
    )
endif (CGREEN_WITH_CXX)

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
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  DESTINATION ${BIN_INSTALL_DIR}
  COMPONENT libraries
)

if (CGREEN_WITH_STATIC_LIBRARY)

  set(CGREEN_STATIC_LIBRARY
    cgreen_static
    CACHE INTERNAL "cgreen static library"
  )

  add_library(${CGREEN_STATIC_LIBRARY} STATIC ${cgreen_SRCS})

  if (CGREEN_WITH_CXX)
    target_link_libraries(${CGREEN_STATIC_LIBRARY} 
      ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES}
    )
  endif (CGREEN_WITH_CXX)

  target_link_libraries(${CGREEN_STATIC_LIBRARY}
    ${CGREEN_LINK_LIBRARIES}
    ${MATH_LIB}
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
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT libraries
  )
endif(CGREEN_WITH_STATIC_LIBRARY)
