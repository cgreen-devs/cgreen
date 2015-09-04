FIND_PACKAGE(ASCIIDOCTOR)

SET(ASCIIDOC_CONFFILE "${CMAKE_SOURCE_DIR}/doc/cgreen_asciidoc.conf")
SET(A2X_ICONS_DIR "${CMAKE_SOURCE_DIR}/images/icons")

OPTION(WITH_HTML "with HTML output" FALSE)
OPTION(WITH_PDF "with PDF output" FALSE)

IF(ASCIIDOCTOR_FOUND)
  FILE(GLOB _docfiles *.asciidoc)
  FOREACH(_file ${_docfiles})
    GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    SET(_in "${_file_we}")
    IF (WITH_HTML)
      SET(_out "${_file_we}.html")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-html"
        COMMAND ${ASCIIDOCTOR_COMMAND}
        -a toc
        -a docinfo -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "Asciidoc ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-html ALL echo
        DEPENDS "${_out}-html"
      )
    ENDIF (WITH_HTML)
    IF (WITH_PDF)
      SET(_out "${_file_we}.pdf")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-pdf"
        COMMAND ${ASCIIDOCTOR_COMMAND}-pdf
        -a toc
        -a docinfo -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "Asciidoc ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-pdf ALL echo
        DEPENDS "${_out}-pdf"
      )
    ENDIF (WITH_PDF)
  ENDFOREACH(_file)
ENDIF(ASCIIDOCTOR_FOUND)
