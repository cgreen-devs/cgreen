# define system dependent compiler flags

include(CheckCCompilerFlag)

set (COMPILER_IS_CLANG FALSE) 
if (${CMAKE_C_COMPILER_ID} MATCHES "Clang") 
  set (COMPILER_IS_CLANG TRUE)
endif (${CMAKE_C_COMPILER_ID} MATCHES "Clang") 

if (UNIX)
  if (CMAKE_COMPILER_IS_GNUCC OR COMPILER_IS_CLANG)
    # add_compile_options(-g -Wall -Wextra -Wunused) # only since CMake 2.8.12, so...
    add_definitions(-g -Wall -Wextra -Wunused)

    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Weffc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

    if (CGREEN_INTERNAL_WITH_GCOV)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs")
    endif (CGREEN_INTERNAL_WITH_GCOV)

    add_definitions(-D_REENTRANT)           # for gmtime_r()
    add_definitions(-D_XOPEN_SOURCE)        # for popen() and pclose()
    add_definitions(-D_XOPEN_SOURCE_EXTENDED) # for strdup(), which isn't part of C99
    add_definitions(-D__STDC_FORMAT_MACROS) # for PRI*PTR format macros, required by C99

    if (NOT CYGWIN)
        # with -fPIC
        check_c_compiler_flag("-fPIC" WITH_FPIC)
        if (WITH_FPIC)
            # add_compile_options(-fPIC) # Only since CMake 2.8.12, so...
            add_definitions(-fPIC)
        endif (WITH_FPIC)
    endif (NOT CYGWIN)

    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
      # with large file support
      execute_process(
        COMMAND
          getconf LFS64_CFLAGS
        OUTPUT_VARIABLE
          _lfs_CFLAGS
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    else (CMAKE_SIZEOF_VOID_P MATCHES "8")
      # with large file support
      execute_process(
        COMMAND
          getconf LFS_CFLAGS
        OUTPUT_VARIABLE
          _lfs_CFLAGS
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      string(REGEX REPLACE "[\r\n]" " " "${_lfs_CFLAGS}" "${${_lfs_CFLAGS}}")

      add_definitions(${_lfs_CFLAGS})
    endif (CMAKE_SIZEOF_VOID_P MATCHES "8")

    check_c_compiler_flag("-D_FORTIFY_SOURCE=2" WITH_FORTIFY_SOURCE)
    if (WITH_FORTIFY_SOURCE)
      add_definitions(-D_FORTIFY_SOURCE=2 -O2)
    endif (WITH_FORTIFY_SOURCE)
  endif (CMAKE_COMPILER_IS_GNUCC OR COMPILER_IS_CLANG)
endif (UNIX)

if (WIN32)
    if (MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    endif (MSVC)
endif (WIN32)

