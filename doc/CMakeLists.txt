FIND_PACKAGE(Asciidoctor)

SET(ASCIIDOC_CONFFILE "${PROJECT_SOURCE_DIR}/doc/cgreen_asciidoc.conf")

OPTION(CGREEN_WITH_HTML_DOCS "with HTML output" FALSE)
OPTION(CGREEN_WITH_PDF_DOCS "with PDF output" FALSE)

IF (CGREEN_WITH_HTML_DOCS AND NOT ASCIIDOCTOR_FOUND)
  MESSAGE(FATAL_ERROR "Can't produce HTML without 'asciidoctor'")
ENDIF (CGREEN_WITH_HTML_DOCS AND NOT ASCIIDOCTOR_FOUND)

IF (CGREEN_WITH_PDF_DOCS AND NOT ASCIIDOCTORPDF_FOUND)
  MESSAGE(FATAL_ERROR "Can't produce PDF without 'asciidoctor-pdf'")
ENDIF (CGREEN_WITH_PDF_DOCS AND NOT ASCIIDOCTORPDF_FOUND)

IF(CGREEN_WITH_HTML_DOCS OR CGREEN_WITH_PDF_DOCS)
  FILE(GLOB _docfiles *.asciidoc)
  FOREACH(_file ${_docfiles})
    GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    SET(_in "${_file_we}")
    IF (CGREEN_WITH_HTML_DOCS)
      SET(_out "${_file_we}.html")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-html"
        COMMAND ${ASCIIDOCTOR_EXECUTABLE}
        -a VERSION=${APPLICATION_VERSION}
        -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "asciidoctor ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-html ALL echo
        DEPENDS "${_out}-html"
      )
    ENDIF (CGREEN_WITH_HTML_DOCS)
    IF (CGREEN_WITH_PDF_DOCS)
      SET(_out "${_file_we}.pdf")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-pdf"
        COMMAND ${ASCIIDOCTORPDF_EXECUTABLE}
        -a toc
        -a VERSION=${APPLICATION_VERSION}
        -a docinfo -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "asciidoctor-pdf ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-pdf ALL echo
        DEPENDS "${_out}-pdf"
      )
    ENDIF (CGREEN_WITH_PDF_DOCS)
  ENDFOREACH(_file)
ENDIF(CGREEN_WITH_HTML_DOCS OR CGREEN_WITH_PDF_DOCS)

IF(UNIX)
    set(MANPAGES man)
    INSTALL(DIRECTORY ${MANPAGES} DESTINATION share/)
ENDIF(UNIX)
