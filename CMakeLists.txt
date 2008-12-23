CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Cgreen)

SET (VERSION_MAJOR "1")
SET (VERSION_MINOR "0")
SET (VERSION_PATCH "alpha3")
SET (VERSION_SUFFIX "")
SET (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX})

SET(MAIN_DIR_NAME "cgreen")

IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")  
SET(CMAKE_C_FLAGS_DEBUG "-g" CACHE STRING
   "Flags used by Debug."
   FORCE)

#IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
#   SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
#ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

SET(CMAKE_C_FLAGS_PROFILING "-g -O0 -Wall -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Compiler Flags" )
SET(CMAKE_SHARED_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags" )
SET(CMAKE_MODULE_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags" )
SET(CMAKE_EXEC_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags" )




MESSAGE(STATUS "${CMAKE_BUILD_TYPE}: Cgreen ${VERSION} will be built to install into ${CMAKE_INSTALL_PREFIX}")

ADD_SUBDIRECTORY(tests tests)
ADD_SUBDIRECTORY(src src)
ADD_SUBDIRECTORY(doc doc)

INCLUDE_DIRECTORIES(${Cgreen_SOURCE_DIR}/include/)

SET(cgreen_sources src/unit.c src/messaging.c src/breadcrumb.c
    src/reporter.c src/assertions.c src/vector.c src/mocks.c src/constraint.c
    src/parameters.c src/text_reporter.c src/cute_reporter.c src/slurp.c)

SET(collector_sources src/collector.c src/vector.c src/slurp.c
    src/collector_test_list.c)

SET(tests_sources tests/all_tests.c tests/messaging_tests.c
    tests/assertion_tests.c tests/vector_tests.c tests/constraint_tests.c
    tests/parameters_test.c tests/mocks_tests.c tests/breadcrumb_tests.c
    tests/slurp_test.c tests/cute_reporter_tests.c)

# Build Cgreen

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_COMPILER_IS_GNUCC)
    ADD_LIBRARY(cgreen-static STATIC ${cgreen_sources})
    ADD_LIBRARY(cgreen SHARED ${cgreen_sources})
    TARGET_LINK_LIBRARIES(cgreen-static m -Wl -Wl,-no-whole-archive -fPIC)
    TARGET_LINK_LIBRARIES(cgreen m -Wl -Wl,-no-whole-archive)
    SET_TARGET_PROPERTIES(cgreen-static PROPERTIES OUTPUT_NAME "cgreen")
    SET_TARGET_PROPERTIES(cgreen PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    SET_TARGET_PROPERTIES(cgreen-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_COMPILER_IS_GNUCC)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/collector.c
    COMMAND lex -B -t ${CMAKE_CURRENT_SOURCE_DIR}/src/collector.l >
    ${CMAKE_CURRENT_SOURCE_DIR}/src/collector.c
)

ADD_EXECUTABLE(collector ${collector_sources})

ADD_EXECUTABLE(tests/all_test ${tests_sources})

ADD_CUSTOM_COMMAND(TARGET tests/all_test POST_BUILD
    COMMAND /bin/mkdir samples    
    COMMAND echo Some stuff > samples/some_file
    COMMENT "Make sample files for tests")

TARGET_LINK_LIBRARIES(tests/all_test cgreen m)

# Build docs
IF(ENABLE_DOCS)

FIND_FILE(XSLT_FOUND xsltproc PATHS /usr/bin)
    IF(NOT XSLT_FOUND)
        MESSAGE(FATAL_ERROR "Command xsltproc is not installed. I can't see it")
    ENDIF(NOT XSLT_FOUND)

FIND_FILE(FOP_FOUND fop PATHS /usr/bin ${CMAKE_SOURCE_DIR}/doc/tools/fop)
    IF(NOT FOP_FOUND)
        MESSAGE(FATAL_ERROR "Command fop is not installed. I can't see it")
    ENDIF(NOT FOP_FOUND)

FIND_FILE(XSL_FOUND docbook.xsl PATHS ${CMAKE_SOURCE_DIR}/doc/tools/xsl/html/)
    IF(NOT XSL_FOUND)
        MESSAGE(FATAL_ERROR "XSL package is not  installed in ${CMAKE_SOURCE_DIR}/doc/tools/xsl")
    ENDIF(NOT XSL_FOUND)

FIND_FILE(DOCBOOK_FOUND docbook.dtd PATHS ${CMAKE_SOURCE_DIR}/doc/tools/docbook-5.0/dtd/)
    IF(NOT DOCBOOK_FOUND)
        MESSAGE(FATAL_ERROR "DOCBOOK package is not  installed in ${CMAKE_SOURCE_DIR}/doc/tools/docbook-5.0")
    ENDIF(NOT DOCBOOK_FOUND)


SET(XSLTPROC ${XSLT_FOUND})
SET(FOP ${FOP_FOUND})
SET(cgreen_doc_en doc/source/cgreen-guide-en.xml)
SET(cgreen_doc_pt_BR doc/source/cgreen-guide-pt_BR.xml)
SET(cgreen_doc cgreen-guide-en cgreen-guide-pt_BR)        
SET(xslstylehtml ${CMAKE_SOURCE_DIR}/doc/source/include/customhtml.xsl)
SET(xslstylefo ${CMAKE_SOURCE_DIR}/doc/source/include/customfo.xsl)
SET(bundled_dir ${CMAKE_BINARY_DIR}/doc/cgreen-bundled)

# PDF

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.fo
    COMMAND ${XSLTPROC}
    ARGS --stringparam fop1.extensions 1 --output ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.fo
        ${xslstylefo} ${CMAKE_SOURCE_DIR}/doc/source/cgreen-guide-pt_BR.xml    
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.pdf
    COMMAND ${FOP}
    ARGS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.fo 
        ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.pdf
    DEPENDS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.fo
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.fo
    COMMAND ${XSLTPROC}
    ARGS --stringparam fop1.extensions 1 --output ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.fo
        ${xslstylefo} ${CMAKE_SOURCE_DIR}/doc/source/cgreen-guide-en.xml
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.pdf
    COMMAND ${FOP}
    ARGS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.fo 
        ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.pdf
    DEPENDS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.fo
)

ADD_CUSTOM_TARGET(pdf ALL
    DEPENDS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.pdf
        ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.pdf
)

# HTML

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.html
    COMMAND ${XSLTPROC}
    ARGS --stringparam  html.stylesheet "docs.css" --output ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.html ${xslstylehtml}
        ${CMAKE_SOURCE_DIR}/doc/source/cgreen-guide-pt_BR.xml
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.html
    COMMAND ${XSLTPROC}
    ARGS --stringparam  html.stylesheet "docs.css" --output ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.html
        ${xslstylehtml} ${CMAKE_SOURCE_DIR}/doc/source/cgreen-guide-en.xml
)

ADD_CUSTOM_TARGET(html ALL
    DEPENDS ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.html ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.html
)

# Bundled html

ADD_CUSTOM_COMMAND(
    OUTPUT ${bundled_dir}/index.html ${bundled_dir}/cgreen_suites.html
        ${bundled_dir}/mock_callbacks.html ${bundled_dir}/cgreen_reporter.html
        ${bundled_dir}/docs.css ${bundled_dir}
    COMMAND mkdir ${bundled_dir}
    COMMAND ${XSLTPROC} --output ${bundled_dir}/index.html 
        ${CMAKE_SOURCE_DIR}/doc/source/include/bundled_docs.xslt
        ${CMAKE_SOURCE_DIR}/doc/source/site/quickstart.xml
    COMMAND ${XSLTPROC} --output ${bundled_dir}/cgreen_suites.html
        ${CMAKE_SOURCE_DIR}/doc/source/include/bundled_docs.xslt
        ${CMAKE_SOURCE_DIR}/doc/source/site/cgreen_suites.xml
    COMMAND ${XSLTPROC} --output ${bundled_dir}/mock_callbacks.html
        ${CMAKE_SOURCE_DIR}/doc/source/include/bundled_docs.xslt
        ${CMAKE_SOURCE_DIR}/doc/source/site/mock_callbacks.xml
    COMMAND ${XSLTPROC} --output ${bundled_dir}/cgreen_reporter.html
        ${CMAKE_SOURCE_DIR}/doc/source/include/bundled_docs.xslt
        ${CMAKE_SOURCE_DIR}/doc/source/site/cgreen_reporter.xml
    COMMAND cp ${CMAKE_SOURCE_DIR}/doc/source/include/docs.css ${bundled_dir}/docs.css
    COMMENT "Building Cgreen bundled html"
)

ADD_CUSTOM_TARGET(bundled-html DEPENDS ${bundled_dir}/index.html)

# Install docs

INSTALL(FILES ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.pdf
    ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.pdf
    ${CMAKE_BINARY_DIR}/doc/cgreen-guide-en.html
    ${CMAKE_BINARY_DIR}/doc/cgreen-guide-pt_BR.html
    ${CMAKE_SOURCE_DIR}/doc/source/include/docs.css
    DESTINATION share/doc/${MAIN_DIR_NAME}-${VERSION}
)

# We can build website too

IF(ENABLE_WEBSITE)

SET(lastcraft_dir ${CMAKE_BINARY_DIR}/doc/lastcraft)
SET(XALAN /usr/bin/Xalan)

ADD_CUSTOM_COMMAND(
    OUTPUT ${lastcraft_dir}/cgreen.php ${lastcraft_dir}/cgreen_suites.php
        ${lastcraft_dir}/mock_callbacks.php ${lastcraft_dir}/cgreen_reporter.php
        ${lastcraft_dir}
    COMMAND mkdir ${lastcraft_dir}
    COMMAND ${XALAN} -o ${bundled_dir}/cgreen.php 
        ${CMAKE_SOURCE_DIR}/doc/source/site/quickstart.xml
        ${CMAKE_SOURCE_DIR}/doc/source/include/lastcraft.xslt
    COMMAND ${XALAN} -o ${bundled_dir}/cgreen_suites.php 
        ${CMAKE_SOURCE_DIR}/doc/source/site/cgreen_suites.xml
        ${CMAKE_SOURCE_DIR}/doc/source/include/lastcraft.xslt
    COMMAND ${XALAN} -o ${bundled_dir}/mock_callbacks.php 
        ${CMAKE_SOURCE_DIR}/doc/source/site/mock_callbacks.xml
        ${CMAKE_SOURCE_DIR}/doc/source/include/lastcraft.xslt
    COMMAND ${XALAN} -o ${bundled_dir}/cgreen_reporter.php 
        ${CMAKE_SOURCE_DIR}/doc/source/site/cgreen_reporter.xml
        ${CMAKE_SOURCE_DIR}/doc/source/include/lastcraft.xslt              
    COMMENT "Building Cgreen website"
)
    ADD_CUSTOM_TARGET(lastcraft-html DEPENDS ${lastcraft_dir}/cgreen.php)
	
ENDIF(ENABLE_WEBSITE)
ENDIF(ENABLE_DOCS)

# Testing
ENABLE_TESTING()
INCLUDE(Dart)

ADD_TEST(tests/all_test ${EXECUTABLE_OUTPUT_PATH}tests/all_test)

# Pack

# We does not finish the collector 
#INSTALL(TARGETS collector RUNTIME DESTINATION bin)

INSTALL(TARGETS cgreen LIBRARY DESTINATION lib)
INSTALL(DIRECTORY include/cgreen DESTINATION include
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    PATTERN "cgreen.h" EXCLUDE
    PATTERN "collector_test_list.h" EXCLUDE
)
INSTALL(FILES include/cgreen/cgreen.h DESTINATION include/)
INSTALL(FILES README INSTALL
    DESTINATION share/doc/${MAIN_DIR_NAME}-${VERSION}
)

SET(CPACK_PACKAGE_NAME "cgreen")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cgreen is a unit test framework for language C")
SET(CPACK_PACKAGE_VENDOR "Cgreen Team")
SET(CPACK_PACKAGE_CONTACT "joaohf@gmail.com")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}.${VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "Cgreen-${VERSION}")
SET(CPACK_SOURCE_IGNORE_FILES 
     "/.*\\\\.tar\\\\.gz"  
     "\\\\.svn/"
     "svn"
     ".*~"
     "doc/tools/xsl/"
     "doc/tools/docbook-5.0"
)
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_GENERATOR STGZ TGZ DEB RPM)
SET(CPACK_SOURCE_GENERATOR TGZ)
INCLUDE(CPack)
