#ifndef DISCOVERER_H
#define DISCOVERER_H

#include <cgreen/vector.h>

#include <stdbool.h>

extern CgreenVector *discover_tests_in(const char *filename, bool verbose);

#endif
