# Required cmake version
cmake_minimum_required(VERSION 2.8.1)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required

set(CMAKE_C_COMPLIER_INIT /usr/bin/gcc)

if (APPLE)
   if (NOT ${CMAKE_C_COMPILER} MATCHES ".*gcc")
      MESSAGE("You should use gcc as the compiler by using 'CC=gcc CXX=g++ cmake ...'")
   endif()
endif ()

project(cgreen C)

enable_testing()

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})
set(APPLICATION_VERSION_MAJOR "1")
set(APPLICATION_VERSION_MINOR "0")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION_STATUS "-beta3")
set(APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_STATUS})
set(LIBRARY_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH})
set(LIBRARY_SOVERSION ${APPLICATION_VERSION_MAJOR})

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

# add definitions
include(DefineCMakeDefaults)
include(DefineCompilerFlags)
include(DefineInstallationPaths)
include(DefineOptions.cmake)
include(DefineCPackConfig)

# add macros
include(MacroAddPlugin)
include(MacroCopyFile)

# config.h checks
include(ConfigureChecks.cmake)
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# check subdirectories
add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tools)
# add_subdirectory(samples)

if (WITH_UNIT_TESTS)
  include(MacroAddUnitTest)
  add_subdirectory(tests)
  add_subdirectory(tools/tests)
endif (WITH_UNIT_TESTS)
