#
# (c)2015 KiCad-Developers
# (c)2015 Brian Sidebotham
#
# Part of the KiCad project
#

unset( ASCIIDOCTOR_FOUND )
unset( _DEBUG )
set( _DBG OFF )

# Attempt to execute the ASCIIDOCTOR utility
execute_process( COMMAND asciidoctor --version
                 RESULT_VARIABLE _ADOC_EXE_RESULT
                 OUTPUT_VARIABLE _ADOC_EXE_OUTPUT
                 ERROR_VARIABLE _ADOC_EXE_ERROR )

set( ASCIIDOCTOR_COMMAND "ASCIIDOCTOR" )

if( _DBG )
    message( STATUS "ASCIIDOCTOR result: ${_ADOC_EXE_RESULT}" )
    message( STATUS "ASCIIDOCTOR outut: ${_ADOC_EXE_OUTPUT}" )
endif()

if( NOT "${_ADOC_EXE_RESULT}" STREQUAL "0" )
    # Attempt to see if a bat file can be run which invokes ASCIIDOCTOR
    execute_process( COMMAND asciidoctor.bat --version
                 RESULT_VARIABLE _ADOC_EXE_RESULT
                 OUTPUT_VARIABLE _ADOC_EXE_OUTPUT
                 ERROR_VARIABLE _ADOC_EXE_ERROR )

    set( ASCIIDOCTOR_COMMAND "asciidoctor.bat" )

    if( NOT "${_ADOC_EXE_RESULT}" STREQUAL "0" )
        set( ASCIIDOCTOR_FOUND FALSE )
    else()
        set( ASCIIDOCTOR_FOUND TRUE )
    endif()
else()
    set( ASCIIDOCTOR_FOUND TRUE )
endif()

if( ASCIIDOCTOR_FOUND )
    # Get the PO4A version number...
    string( REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" ADOC_VERSION "${_ADOC_EXE_OUTPUT}" )
    message( STATUS "ASCIIDOCTOR Found v${ADOC_VERSION}" )
else()
    message( STATUS "ASCIIDOCTOR NOT FOUND" )
endif()

macro( asciidoctor ARGS )

endmacro()
