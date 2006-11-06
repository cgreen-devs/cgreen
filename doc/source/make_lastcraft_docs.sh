#!/bin/sh
Xalan -o ../lastcraft/cgreen.php quickstart.xml lastcraft.xslt
Xalan -o ../lastcraft/cgreen_suites.php cgreen_suites.xml lastcraft.xslt
Xalan -o ../lastcraft/mock_callbacks.php mock_callbacks.xml lastcraft.xslt
Xalan -o ../lastcraft/cgreen_reporter.php cgreen_reporter.xml lastcraft.xslt
