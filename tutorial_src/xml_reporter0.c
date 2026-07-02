#include <cgreen/reporter.h>

#include "xml_reporter.h"

TestReporter *create_xml_reporter() {
    TestReporter *reporter = create_reporter();
    return reporter;
}
