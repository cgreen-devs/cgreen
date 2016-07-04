FIND_PACKAGE(Asciidoctor)

SET(ASCIIDOC_CONFFILE "${PROJECT_SOURCE_DIR}/doc/cgreen_asciidoc.conf")

OPTION(WITH_HTML "with HTML output" FALSE)
OPTION(WITH_PDF "with PDF output" FALSE)

IF (WITH_HTML AND NOT ASCIIDOCTOR_FOUND)
  MESSAGE(FATAL_ERROR "Can't produce HTML without 'asciidoctor'")
ENDIF (WITH_HTML AND NOT ASCIIDOCTOR_FOUND)

IF (WITH_PDF AND NOT ASCIIDOCTORPDF_FOUND)
  MESSAGE(FATAL_ERROR "Can't produce PDF without 'asciidoctor-pdf'")
ENDIF (WITH_PDF AND NOT ASCIIDOCTORPDF_FOUND)

IF(WITH_HTML OR WITH_PDF)
  FILE(GLOB _docfiles *.asciidoc)
  FOREACH(_file ${_docfiles})
    MESSAGE("_file is " ${_file})
    GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    SET(_in "${_file_we}")
    IF (WITH_HTML)
      SET(_out "${_file_we}.html")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-html"
        COMMAND ${ASCIIDOCTOR_EXECUTABLE}
        -a toc
        -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "asciidoctor ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-html ALL echo
        DEPENDS "${_out}-html"
      )
    ENDIF (WITH_HTML)
    IF (WITH_PDF)
      SET(_out "${_file_we}.pdf")
      ADD_CUSTOM_COMMAND(
        OUTPUT "${_out}-pdf"
        COMMAND ${ASCIIDOCTORPDF_EXECUTABLE}
        -a toc
        -a docinfo -o ${_out} ${_file}
        DEPENDS ${_file}
        COMMENT "asciidoctor-pdf ${_in}"
      )
      ADD_CUSTOM_TARGET(${_in}-pdf ALL echo
        DEPENDS "${_out}-pdf"
      )
    ENDIF (WITH_PDF)
  ENDFOREACH(_file)
ENDIF(WITH_HTML OR WITH_PDF)

IF(UNIX)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cgreen.5 DESTINATION ${CMAKE_INSTALL_MANDIR}/man5)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cgreen-runner.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
ENDIF(UNIX)

