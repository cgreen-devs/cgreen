#ifndef XML_REPORTER_INTERNAL_H
#define XML_REPORTER_INTERNAL_H

#include <libxml/tree.h>

#include <cgreen/xml_reporter.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
typedef int XmlPrinter(xmlDocPtr);
extern void set_libxml_reporter_printer(TestReporter *reporter, XmlPrinter *printer);

#ifdef __cplusplus
}
#endif

#endif
