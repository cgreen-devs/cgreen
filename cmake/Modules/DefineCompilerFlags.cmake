# define system dependent compiler flags

include(CheckCCompilerFlag)

if (USE_W32API)
  add_definitions(-mno-cygwin)
endif (USE_W32API)

if (UNIX AND NOT WIN32) # All *nix except Cygwin
  if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Wextra -Wmissing-prototypes -Wdeclaration-after-statement -Wunused)

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

    check_c_compiler_flag("-fstack-protector" WITH_STACK_PROTECTOR)
    if (WITH_STACK_PROTECTOR)
      add_definitions(-fstack-protector)
    endif (WITH_STACK_PROTECTOR)

    check_c_compiler_flag("-D_FORTIFY_SOURCE=2" WITH_FORTIFY_SOURCE)
    if (WITH_FORTIFY_SOURCE)
      add_definitions(-D_FORTIFY_SOURCE=2)
    endif (WITH_FORTIFY_SOURCE)
  endif (CMAKE_COMPILER_IS_GNUCC)
endif (UNIX AND NOT WIN32)
