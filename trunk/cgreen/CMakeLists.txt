project(cgreen C)

# Required cmake version
cmake_minimum_required(VERSION 2.6.0)

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})
set(APPLICATION_VERSION "1.0.0-beta2")
set(LIBRARY_VERSION "1.0.0")
set(LIBRARY_SOVERSION "1")

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

if (WIN32)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif (WIN32)

# check subdirectories
add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(samples)

if (WITH_UNIT_TESTS)
  include(MacroAddUnitTest)
  add_subdirectory(tests)
endif (WITH_UNIT_TESTS)

#

