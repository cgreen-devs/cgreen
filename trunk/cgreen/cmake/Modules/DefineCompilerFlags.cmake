# define system dependent compiler flags

include(CheckCCompilerFlag)

if (UNIX) 
  if (CMAKE_COMPILER_IS_GNUCC)
    if (WITH_CXX)
       add_definitions(-g -Weffc++ -Wall -Wextra -Wunused)
    else()
       add_definitions(-g -std=c99 -Wall -Wextra -Wmissing-prototypes -Wunused)
    endif (WITH_CXX)

    add_definitions(-D_REENTRANT)         # for gmtime_r()
    add_definitions(-DUSE_XOPEN_EXTENDED) # for strdup(), which isn't part of C99

    # with -fPIC
    check_c_compiler_flag("-fPIC" WITH_FPIC)
    if (WITH_FPIC)
      add_definitions(-fPIC)
    endif (WITH_FPIC)

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
      add_definitions(-D_FORTIFY_SOURCE=2)
    endif (WITH_FORTIFY_SOURCE)
  endif (CMAKE_COMPILER_IS_GNUCC)
endif (UNIX)
