## Process this with automake to create Makefile.in

AUTOMAKE_OPTIONS = foreign

docdir = $(datadir)/doc/$(PACKAGE)-$(VERSION)

doc_DATA = source/en/cgreen-guide-en.xml source/en/suites.xml source/en/mock_callbacks.xml source/en/quickstart.xml source/en/reporter.xml

EXTRA_DIST = $(doc_DATA)  $(programmers_sources)
XSLSTYLEHTML=source/include/customhtml.xsl
XSLSTYLEFO=source/include/customfo.xsl


%.html:  source/en/cgreen-guide-en.xml
	@echo "Auto-generating Cgreen's Guide HTML...";
	mkdir cgreen-guide	
	xsltproc  --stringparam  html.stylesheet "docs.css"  --output  $@  $(XSLSTYLEHTML)  $<

html:  cgreen-guide/cgreen-guide-en.html
	cp source/include/docs.css cgreen-guide

%.fo: source/en/cgreen-guide-en.xml
	@echo "Auto-generating Cgreen's Guide FO...";
	xsltproc  --stringparam fop1.extensions 1 --output  $@  $(XSLSTYLEFO)  $<

fo: cgreen-guide-en.fo

%.pdf: cgreen-guide-en.fo
	@echo "Auto-generating Cgreen's Guide FO...";
	fop $< $@

pdf: cgreen-guide-en.pdf
	
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
	
