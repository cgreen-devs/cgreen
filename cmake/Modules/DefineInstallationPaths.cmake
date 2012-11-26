if (UNIX)
  IF (NOT APPLICATION_NAME)
    MESSAGE(STATUS "${PROJECT_NAME} is used as APPLICATION_NAME")
    SET(APPLICATION_NAME ${PROJECT_NAME})
  ENDIF (NOT APPLICATION_NAME)

  # The following are subdirectories relative to CMAKE_INSTALL_PREFIX where stuff will be installed to
  SET(BIN_INSTALL_DIR
    "bin"
    CACHE PATH "The ${APPLICATION_NAME} binary install dir (default <prefix>/bin)"
  )

  SET(SBIN_INSTALL_DIR
    "sbin"
    CACHE PATH "The ${APPLICATION_NAME} sbin install dir (default <prefix>/sbin)"
  )

  SET(LIB_INSTALL_DIR
    "lib${LIB_SUFFIX}"
    CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is <prefix>/lib)"
  )

  # Suffix for Linux
  SET(LIB_SUFFIX
    CACHE STRING "Define suffix of directory for libraries (32/64)"
  )

  SET(LIBEXEC_INSTALL_DIR
    "libexec"
    CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is <prefix>/libexec)"
  )

  SET(INCLUDE_INSTALL_DIR
    "include"
    CACHE PATH "The subdirectory to the header prefix (default <prefix>/include)"
  )

  SET(SHARE_INSTALL_PREFIX
    "share"
    CACHE PATH "Base directory for files which go to share/"
  )

  SET(HTML_INSTALL_DIR
    "doc/HTML"
    CACHE PATH "The HTML install dir for documentation (default <prefix>/doc/html)"
  )

#  SET(LOCALE_INSTALL_DIR
#    "${SHARE_INSTALL_PREFIX}/locale"
#    CACHE PATH "The install dir for translations (default <prefix>/share/locale)"
#  )

#  SET(MAN_INSTALL_DIR
#    "${SHARE_INSTALL_PREFIX}/man"
#    CACHE PATH "The ${APPLICATION_NAME} man install dir (default <prefix>/man)"
#  )

#  SET(INFO_INSTALL_DIR
#    "${SHARE_INSTALL_PREFIX}/info"
#    CACHE PATH "The ${APPLICATION_NAME} info install dir (default <prefix>/info)"
#  )
endif (UNIX)

# We don't support WIN32 yet (and cygwin is *not* WIN32!!)
#if (WIN32)
#  # Same same
#  set(BIN_INSTALL_DIR "." CACHE PATH "-")
#  set(SBIN_INSTALL_DIR "." CACHE PATH "-")
#  set(LIB_INSTALL_DIR "lib" CACHE PATH "-")
#  set(INCLUDE_INSTALL_DIR "include" CACHE PATH "-")
#  set(PLUGIN_INSTALL_DIR "plugins" CACHE PATH "-")
#  set(HTML_INSTALL_DIR "doc/HTML" CACHE PATH "-")
#  set(ICON_INSTALL_DIR "." CACHE PATH "-")
#  set(SOUND_INSTALL_DIR "." CACHE PATH "-")
#  set(LOCALE_INSTALL_DIR "lang" CACHE PATH "-")
#endif (WIN32)

