## Process this with automake to create Makefile.in

AUTOMAKE_OPTIONS = foreign

docdir = $(datadir)/doc/$(PACKAGE)-$(VERSION)
progguidedir = $(docdir)/programmers-guide

doc_DATA = source/cgreen_suites.xml source/mock_callbacks.xml source/quickstart.xml source/cgreen_reporter.xml

programmers_sources = source/cgreen_suites.xml source/mock_callbacks.xml source/quickstart.xml source/cgreen_reporter.xml

EXTRA_DIST = $(doc_DATA)  $(programmers_sources)

all: programmers-guide


programmers-guide: 
	@echo "Converting XML to HTML...";
	xsltproc $(top_srcdir)/doc/source/bundled_docs.xslt $(top_srcdir)/doc/source/cgreen_suites.xml > $(top_srcdir)/doc/cgreen_suites.html
	xsltproc $(top_srcdir)/doc/source/bundled_docs.xslt $(top_srcdir)/doc/source/mock_callbacks.xml > $(top_srcdir)/doc/mock_callbacks.html
	xsltproc $(top_srcdir)/doc/source/bundled_docs.xslt $(top_srcdir)/doc/source/quickstart.xml > $(top_srcdir)/doc/quickstart.html
	xsltproc $(top_srcdir)/doc/source/bundled_docs.xslt $(top_srcdir)/doc/source/cgreen_reporter.xml > $(top_srcdir)/doc/cgreen_reporter.html

distclean: clean
	rm -f *.html


