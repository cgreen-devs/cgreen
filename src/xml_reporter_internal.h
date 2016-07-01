#ifndef XML_REPORTER_INTERNAL_H
#define XML_REPORTER_INTERNAL_H

#include <stdio.h>

#include <cgreen/xml_reporter.h>

typedef int XmlPrinter(FILE *, const char *format, ...);
extern void set_xml_reporter_printer(TestReporter *reporter, XmlPrinter *printer);

#endif
