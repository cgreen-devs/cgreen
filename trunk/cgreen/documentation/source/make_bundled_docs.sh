#!/bin/sh
xsltproc bundled_docs.xslt quickstart.xml > ../index.html
xsltproc bundled_docs.xslt cgreen_suites.xml > ../cgreen_suites.html
