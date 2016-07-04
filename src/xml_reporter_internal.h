#ifndef XML_REPORTER_INTERNAL_H
#define XML_REPORTER_INTERNAL_H

#include <cgreen/xml_reporter.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef int XmlPrinter(FILE *, const char *format, ...);
extern void set_xml_reporter_printer(TestReporter *reporter, XmlPrinter *printer);

#ifdef __cplusplus
}
#endif

#endif
