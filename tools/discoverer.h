#ifndef DISCOVERER_H
#define DISCOVERER_H

#include <cgreen/vector.h>

#include <stdbool.h>

// We want to run our tests with cgreen, of course, so in order to not
// cause conflicts with the function in the library itself we rename
// it here when we are running unittests. Also see Makefile.
#ifdef UNITTESTING
#define discover_tests_in(x,y) unittesting_discover_tests_in(x,y)
#endif

extern CgreenVector *discover_tests_in(const char *filename, bool verbose);

#endif
