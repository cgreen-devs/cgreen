# This subdirectory verifies that the whole user level api compiles
# using only the publiced includes by only compiling a test file
# containing use of the whole api

if (WITH_CXX)
  project(api CXX)
else()
  project(api C)
endif (WITH_CXX)

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS})

set(api_SRCS
  core_api.c
  no_context_api.c
  context_api.c
)

if (WITH_CXX)
  SET_SOURCE_FILES_PROPERTIES( ${api_SRCS} PROPERTIES LANGUAGE CXX )
endif (WITH_CXX)


set(CGREEN_API_LIBRARY
  cgreen_api
  CACHE INTERNAL "cgreen library of user level api verification source"
)

add_library(${CGREEN_API_LIBRARY} SHARED ${api_SRCS})

if (WITH_CXX)
   target_link_libraries(${CGREEN_API_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
else (WITH_CXX)
   target_link_libraries(${CGREEN_API_LIBRARY} ${CGREEN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
endif (WITH_CXX)
