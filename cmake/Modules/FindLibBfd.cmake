# - Try to find libbfd
# Once done this will define
#
#  LIBBFD_FOUND - system has libbfd
#  LIBBFD_INCLUDE_DIRS - the libbfd include directory
#  LIBBFD_LIBRARIES - Link these to use libbfd
#  LIBBFD_DEFINITIONS - Compiler switches required for using libbfd
#
# Based on:
#
#  Copyright (c) 2008 Bernhard Walle <bernhard.walle@gmx.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
#  Remove libiberty and libz lookups /thoni56 2021-12-29


if (LIBBFD_LIBRARIES AND LIBBFD_INCLUDE_DIRS)
  set (LIBBFD_FIND_QUIETLY TRUE)
endif ()

if (CMAKE_HOST_APPLE)
  # Prefer brew/gcc compilation since that allows cgreen-runner be built with libbfd
  find_program(BREW_FOUND NAMES brew)
  if (BREW_FOUND)
    set(HOMEBREW_HOME "/usr/local")
    exec_program(brew ARGS "--prefix" OUTPUT_VARIABLE HOMEBREW_HOME)
    set(HOMEBREW_ONLY_SEARCH_PATH "${HOMEBREW_HOME}/opt/binutils")
    set(HOMEBREW_ONLY_SEARCH_PATH_FOR_INCLUDE "${HOMEBREW_ONLY_SEARCH_PATH}/include")
    set(HOMEBREW_ONLY_SEARCH_PATH_FOR_LIB "${HOMEBREW_ONLY_SEARCH_PATH}/lib")
  endif()
endif()

#message("HOMEBREW_ONLY_SEARCH_PATH = ${HOMEBREW_ONLY_SEARCH_PATH}")

find_path (LIBBFD_INCLUDE_DIRS
  NAMES
    bfd.h
    dis-asm.h
  PATHS
    /usr/include
    /usr/local/include
    /opt/local/include
    /opt/include
    ${HOMEBREW_ONLY_SEARCH_PATH_FOR_INCLUDE}
    ENV CPATH)
#message("LIBBFD_INCLUDE_DIRS = ${LIBBFD_INCLUDE_DIRS}")

# Ugly, yes ugly...
find_library (LIBBFD_LIBRARY
  NAMES
    bfd
  PATHS
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /usr/include
    /opt/local/lib
    /opt/usr/lib64
    ${HOMEBREW_ONLY_SEARCH_PATH_FOR_LIB}
    ENV LIBRARY_PATH
    ENV LD_LIBRARY_PATH)
#message("LIBBFD_LIBRARY = ${LIBBFD_LIBRARY}")

if (CMAKE_HOST_APPLE OR CYGWIN)
  # These are not required on Ubuntu, MSYS2
  # but are required on Homebrew, Cygwin
  # Homebrew requires a re-install of binutils with '--enable-install-libiberty'
  find_library (LIBIBERTY_LIBRARY
    NAMES
    iberty
    PATHS
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /usr/include
    /opt/local/lib
    /opt/usr/lib64
    ${HOMEBREW_ONLY_SEARCH_PATH_FOR_LIB}
    ENV LIBRARY_PATH
    ENV LD_LIBRARY_PATH)
  #message("LIBIBERTY_LIBRARY = ${LIBIBERTY_LIBRARY}")

  find_library (LIBZ_LIBRARY
    NAMES
    z
    PATHS
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /usr/include
    /opt/local/lib
    /opt/usr/lib64
    ${HOMEBREW_ONLY_SEARCH_PATH_FOR_LIB}
    ENV LIBRARY_PATH
    ENV LD_LIBRARY_PATH)
  #message("LIBZ_LIBRARY = ${LIBZ_LIBRARY}")
endif()

# required on Homebrew
find_library (LIBIBERTY_LIBRARY
  NAMES
    iberty
  PATHS
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /usr/include
    /opt/local/lib
    /opt/usr/lib64
    /usr/local/opt/binutils/lib
    ENV LIBRARY_PATH
    ENV LD_LIBRARY_PATH)

find_library (LIBZ_LIBRARY
  NAMES
    z
  PATHS
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /usr/include
    /opt/local/lib
    /opt/usr/lib64
    /usr/local/opt/binutils/lib
    ENV LIBRARY_PATH
    ENV LD_LIBRARY_PATH)

include (FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIBBFD_FOUND to TRUE if all listed variables are TRUE
if (CMAKE_HOST_APPLE)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibBfd DEFAULT_MSG
    LIBBFD_LIBRARY
    LIBIBERTY_LIBRARY
    LIBZ_LIBRARY
    LIBBFD_INCLUDE_DIRS)
else()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibBfd DEFAULT_MSG
    LIBBFD_LIBRARY
    LIBBFD_INCLUDE_DIRS)
endif()

set(LIBBFD_LIBRARIES "${LIBBFD_LIBRARY}" "${LIBIBERTY_LIBRARY}" "${LIBZ_LIBRARY}")
mark_as_advanced(LIBBFD_INCLUDE_DIRS LIBBFD_LIBRARIES LIBBFD_BFD_LIBRARY)
