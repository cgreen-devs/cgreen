#!/bin/sh
Xalan -o ../index.html quickstart.xml bundled_docs.xslt
Xalan -o ../cgreen_suites.html cgreen_suites.xml bundled_docs.xslt
Xalan -o ../mock_callbacks.html mock_callbacks.xml bundled_docs.xslt
