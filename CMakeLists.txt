# Required cmake version
cmake_minimum_required(VERSION 2.8.5)

project(cgreen)

if (NOT(CMAKE_MAJOR_VERSION LESS 3) AND APPLE)
  if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW) # CMake 3.0 to use @rpath on MacOSX libraries
  endif()
endif()

find_package (Threads)

enable_testing()

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})
set(APPLICATION_VERSION_MAJOR "1")
set(APPLICATION_VERSION_MINOR "2")
set(APPLICATION_VERSION_PATCH "0")

set(APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_STATUS})
add_definitions(-DVERSION="${APPLICATION_VERSION}")

set(LIBRARY_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH})
set(LIBRARY_SOVERSION ${APPLICATION_VERSION_MAJOR})

include(GNUInstallDirs)

if(MSVC)
  # these have sensible defaults on other platforms
  set(CMAKE_INSTALL_BINDIR ".")
  set(CMAKE_INSTALL_LIBDIR ".")
endif(MSVC)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/Modules
)

# add definitions
include(DefineCMakeDefaults)
include(DefineCompilerFlags)
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
if (UNIX OR MSYS)
  # reflective runner only supported on UNIX/binutils platforms
  add_subdirectory(tools)
endif(UNIX OR MSYS)
# add_subdirectory(samples)

if (CGREEN_WITH_UNIT_TESTS)
  include(MacroAddUnitTest)
  include(MacroAddTest)
  add_subdirectory(tests)
  if (UNIX OR MSYS)
    # reflective runner only supported on UNIX/binutils platforms
    add_subdirectory(tools/tests)
  endif(UNIX OR MSYS)
endif (CGREEN_WITH_UNIT_TESTS)

# add custom 'check' target to run tests with output-on-failure
if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure
        --build-config "$<CONFIGURATION>")
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure)
endif()

IF(CGREEN_INTERNAL_WITH_GCOV)
  IF(CMAKE_C_COMPILER_ID STREQUAL GNU)
    include(CodeCoverage)
    add_custom_target(coverage
        COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' -o coverage.info
        COMMAND ${GENHTML_PATH} -o coverage coverage.info
    )
  ENDIF()
ELSE()
  add_custom_target(coverage
      COMMAND echo "WARNING: Configure CGREEN_INTERNAL_WITH_GCOV to get coverage")
ENDIF()


#### Begin cgreen package configuration steps. ####

# After install other CMake projects can
# use find_package( cgreen )

set( CONFIG_INSTALL_DIR
        "lib/cmake/${APPLICATION_NAME}" )

set( PROJECT_CONFIG_IN
        "${CMAKE_CURRENT_SOURCE_DIR}/cgreen-config.cmake.in" )

set( VERSION_CONFIG_IN
        "${CMAKE_CURRENT_SOURCE_DIR}/cgreen-config-version.cmake.in" )

set( PROJECT_CONFIG
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cgreen-config.cmake" )

set( VERSION_CONFIG
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cgreen-config-version.cmake" )

configure_file( ${PROJECT_CONFIG_IN}
        "${PROJECT_CONFIG}" @ONLY )

configure_file( ${VERSION_CONFIG_IN}
        "${VERSION_CONFIG}" @ONLY )

install(FILES
        "${PROJECT_CONFIG}" "${VERSION_CONFIG}"
        DESTINATION
        "${CONFIG_INSTALL_DIR}" )

#### End cgreen package configuration steps. ####
