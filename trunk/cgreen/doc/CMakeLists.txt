
FIND_PACKAGE(Asciidoc)

SET(ASCIIDOC_CONFFILE "${CMAKE_SOURCE_DIR}/doc/cgreen_asciidoc.conf")
SET(A2X_ICONS_DIR "${CMAKE_SOURCE_DIR}/images/icons")

OPTION(WITH_HTML "with HTML output" FALSE)
OPTION(WITH_PDF "with PDF output" FALSE)

IF(ASCIIDOC_FOUND AND WITH_HTML)
  FILE(GLOB _docfiles *.asciidoc)
  FOREACH(_file ${_docfiles})
    GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    SET(_in "${_file_we}")
    SET(_out "${_file_we}.html")
    IF (NOT "${_in}" STREQUAL "CMakeLists")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-html"
        COMMAND ${ASCIIDOC_EXECUTABLE} -f ${ASCIIDOC_CONFFILE}
        -a toc
        -a docinfo -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "Asciidoc ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-html ALL echo
        DEPENDS "${_out}-html"
      )
    ENDIF (NOT "${_in}" STREQUAL "CMakeLists")
  ENDFOREACH(_file)
ENDIF(ASCIIDOC_FOUND AND WITH_HTML)

IF(A2X_FOUND AND WITH_PDF)
  FILE(GLOB _docfiles *.asciidoc)
  FOREACH(_file ${_docfiles})
    GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    SET(_in "${_file_we}")
    SET(_out "${_file_we}.html")
    
    IF("${_in}" MATCHES "pt_BR")
      SET(LANG "pt_BR")
    ELSE("${_in}" MATCHES "pt_BR")
      SET(LANG "en_US")
    ENDIF("${_in}" MATCHES "pt_BR")

    IF (NOT "${_in}" STREQUAL "CMakeLists")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-pdf"
        COMMAND ${A2X_EXECUTABLE} -L -f pdf -a toc --fop -d book
          --icons-dir ${A2X_ICONS_DIR}
          -a lang=${LANG}
          -a docinfo
          -a toclevels=3
          -v
          --destination-dir ${CMAKE_BINARY_DIR}/doc ${_file} 
        DEPENDS ${_file}
        COMMENT "a2x ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-pdf ALL echo
        DEPENDS "${_out}-pdf"
      )
    ENDIF (NOT "${_in}" STREQUAL "CMakeLists")
  ENDFOREACH(_file)
ENDIF(A2X_FOUND AND WITH_PDF)
