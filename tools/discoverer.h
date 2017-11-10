#ifndef DISCOVERER_H
#define DISCOVERER_H

#include <cgreen/vector.h>

#include <stdbool.h>

#ifdef UNITTESTING
#define discover_tests_in(x,y) unittesting_discover_tests_in(x,y)
#endif

extern CgreenVector *discover_tests_in(const char *filename, bool verbose);

#endif
