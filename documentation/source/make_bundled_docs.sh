#!/bin/sh
xsltproc bundled_docs.xslt quickstart.xml > ../index.html
xsltproc bundled_docs.xslt cgreen_suites.xml > ../cgreen_suites.html
xsltproc bundled_docs.xslt mock_callbacks.xml > ../mock_callbacks.html
xsltproc bundled_docs.xslt cgreen_reporter.xml > ../cgreen_reporter.html
