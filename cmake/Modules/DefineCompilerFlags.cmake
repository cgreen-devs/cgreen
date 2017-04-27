# define system dependent compiler flags

include(CheckCCompilerFlag)

set (COMPILER_IS_CLANG FALSE)
if (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  set (COMPILER_IS_CLANG TRUE)
endif (${CMAKE_C_COMPILER_ID} MATCHES "Clang")

if (UNIX)
  if (CMAKE_COMPILER_IS_GNUCC OR COMPILER_IS_CLANG)
    # add_compile_options(-Wall -Wextra -Wunused) # only since CMake 2.8.12, so...
    add_definitions(-Wall -Wextra -Wunused)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -Weffc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wstrict-prototypes")

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

    check_c_compiler_flag("-D_FORTIFY_SOURCE=2" WITH_FORTIFY_SOURCE)
    if (WITH_FORTIFY_SOURCE)
      add_definitions(-D_FORTIFY_SOURCE=2)
    endif (WITH_FORTIFY_SOURCE)

    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_definitions(-O)
    else ()
      check_c_compiler_flag("-Og" WITH_OPTIMIZE_FOR_DEBUG)
      if (WITH_OPTIMIZE_FOR_DEBUG)
        add_definitions(-Og)
      endif ()
    endif ()
  endif (CMAKE_COMPILER_IS_GNUCC OR COMPILER_IS_CLANG)
endif (UNIX)

if (WIN32)
    if (MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    endif (MSVC)
endif (WIN32)
