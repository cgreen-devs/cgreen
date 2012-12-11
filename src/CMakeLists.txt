if (WITH_CXX)
  project(cgreen-library CXX)
else()
  project(cgreen-library C)
endif (WITH_CXX)

set(CGREEN_PUBLIC_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  CACHE INTERNAL "cgreen public include directories"
)

set(CGREEN_PRIVATE_INCLUDE_DIRS
  ${CMAKE_BINARY_DIR}
)

set(CGREEN_SHARED_LIBRARY
  cgreen_shared
  CACHE INTERNAL "cgreen shared library"
)

set(CGREEN_STATIC_LIBRARY
  cgreen_static
  CACHE INTERNAL "cgreen static library"
)

set(CGREEN_LINK_LIBRARIES
)

set(cgreen_SRCS
  assertions.c
  boxed_double.c
  breadcrumb.c
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
  vector.c
)

set(CGREEN_LIBRARY_NAME cgreen)

if (WITH_CXX)
  set(cgreen_SRCS ${cgreen_SRCS} 
    cpp_assertions.cpp
    cpp_constraint.cpp
  )
  set(CGREEN_LIBRARY_NAME cgreen++)
  SET_SOURCE_FILES_PROPERTIES( ${cgreen_SRCS} PROPERTIES LANGUAGE CXX )
endif (WITH_CXX)
 
include_directories(
  ${CGREEN_PUBLIC_INCLUDE_DIRS}
  ${CGREEN_PRIVATE_INCLUDE_DIRS}
)

### cgreen
add_library(${CGREEN_SHARED_LIBRARY} SHARED ${cgreen_SRCS})

if (WITH_CXX)
   target_link_libraries(${CGREEN_SHARED_LIBRARY} ${CGREEN_LINK_LIBRARIES} stdc++ pthread)
else (WITH_CXX)
   target_link_libraries(${CGREEN_SHARED_LIBRARY} ${CGREEN_LINK_LIBRARIES} pthread)
endif (WITH_CXX)

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
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  COMPONENT libraries
)

add_library(${CGREEN_STATIC_LIBRARY} STATIC ${cgreen_SRCS})

target_link_libraries(${CGREEN_STATIC_LIBRARY} ${CGREEN_LINK_LIBRARIES} stdc++)

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
  TARGETS
    ${CGREEN_STATIC_LIBRARY}
  DESTINATION
    ${LIB_INSTALL_DIR}
  COMPONENT
    libraries
)
