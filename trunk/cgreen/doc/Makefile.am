## Process this with automake to create Makefile.in

AUTOMAKE_OPTIONS = foreign

docdir = $(datadir)/doc/$(PACKAGE)-$(VERSION)

doc_DATA = source/cgreen-guide-en.xml source/en/suites.xml \
           source/en/mock_callbacks.xml source/en/quickstart.xml \
           source/en/reporter.xml \
           source/cgreen-guide-pt_BR.xml source/pt_BR/suites.xml \
           source/pt_BR/mock_callbacks.xml source/pt_BR/quickstart.xml \
           source/pt_BR/reporter.xml \
           source/include/bundled_docs.xslt source/include/bundled_map.xml \
           source/include/copying-fdl.xml source/include/customfo.xsl \
           source/include/customhtml.xsl source/include/docs.css \
           source/include/lastcraft.xslt source/include/site_map.xml \
           source/site/cgreen_reporter.xml source/site/cgreen_suites.xml \
           source/site/mock_callbacks.xml source/site/quickstart.xml \
           source/tutorial_test/all_tests.c source/tutorial_test/crash_test.c \
           source/tutorial_test/first_test.c source/tutorial_test/person.h \
           source/tutorial_test/person_test.c source/tutorial_test/stream.c \
           source/tutorial_test/stream_test.c source/tutorial_test/strlen_test.c \
           source/tutorial_test/test_as_xml.c source/tutorial_test/words.c \
           source/tutorial_test/words.h source/tutorial_test/words_test.c \
           source/tutorial_test/xml_reporter.c source/tutorial_test/xml_reporter.h
           
EXTRA_DIST = $(doc_DATA) cgreen-guide
XSLSTYLEHTML=source/include/customhtml.xsl
XSLSTYLEFO=source/include/customfo.xsl

html:  cgreen-guide-en.html
	cp source/include/docs.css cgreen-guide

%.html:  
	@echo "Auto-generating Cgreen's Guide HTML...";
	mkdir cgreen-guide	
	xsltproc  --stringparam  html.stylesheet "docs.css"  --output  cgreen-guide/$@  $(XSLSTYLEHTML)  source/$*.xml

pdf: cgreen-guide-en.pdf cgreen-guide-pt_BR.pdf

fo: cgreen-guide-en.fo

%.pdf: %.fo
	@echo "Auto-generating Cgreen's Guide FO..."
	fop $< $@

%.fo:
	@echo "Auto-generating Cgreen's Guide xlsproc..."
	xsltproc  --stringparam fop1.extensions 1 --output  $@  $(XSLSTYLEFO)  source/$*.xml

bundled-html:
	mkdir cgreen-bundled
	xsltproc --output cgreen-bundled/index.html source/include/bundled_docs.xslt  source/site/quickstart.xml
	xsltproc --output cgreen-bundled/cgreen_suites.html source/include/bundled_docs.xslt  source/site/cgreen_suites.xml
	xsltproc --output cgreen-bundled/mock_callbacks.html source/include/bundled_docs.xslt  source/site/mock_callbacks.xml
	xsltproc --output cgreen-bundled/cgreen_reporter.html source/include/bundled_docs.xslt  source/site/cgreen_reporter.xml
	cp source/include/docs.css cgreen-bundled

bundled-html-xlan:
	mkdir cgreen-bundled
	Xalan -o cgreen-bundled/index.html quickstart.xml source/include/bundled_docs.xslt
	Xalan -o cgreen-bundled/cgreen_suites.html cgreen_suites.xml source/include/bundled_docs.xslt
	Xalan -o cgreen-bundled/mock_callbacks.html mock_callbacks.xml source/include/bundled_docs.xslt
	Xalan -o cgreen-bundled/cgreen_reporter.html cgreen_reporter.xml source/include/bundled_docs.xslt
	cp source/include/docs.css cgreen-bundled

lastcraft-html:
	mkdir lastcraft
	Xalan -o lastcraft/cgreen.php quickstart.xml source/include/lastcraft.xslt
	Xalan -o lastcraft/cgreen_suites.php cgreen_suites.xml source/include/lastcraft.xslt
	Xalan -o lastcraft/mock_callbacks.php mock_callbacks.xml source/include/lastcraft.xslt
	Xalan -o lastcraft/cgreen_reporter.php cgreen_reporter.xml source/include/lastcraft.xslt	

all: html pdf

distclean: clean
	rm -f *.pdf
	rm -f *.fo
	rm -rf cgreen-guide
	rm -rf cgreen-bundled
	rm -rf lastcraft

dist-hook: html

