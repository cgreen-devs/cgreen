#include "discoverer.h"

#include "io.h"

// bfd.h needs to have PACKAGE and PACKAGE_VERSION defined
#define PACKAGE "cgreen-runner"
#define PACKAGE_VERSION "unknown"
#include <bfd.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <cgreen/internal/unit_implementation.h>

#include "../src/utils.h"


#include "test_item.h"

static const char *cgreen_spec_start_of(const char *line) {
    return strstr(line, CGREEN_SPEC_PREFIX CGREEN_SEPARATOR);
}

static bool contains_cgreen_spec(const char *line) {
    return cgreen_spec_start_of(line) != NULL;
}

static void add_all_tests_from(asymbol **symbols, long symbol_count, CgreenVector *tests, bool verbose) {

    for (long index = 0; index < symbol_count; ++index, ++symbols) {
        bfd *cur_bfd;

        if ((*symbols != NULL)
            && ((cur_bfd = bfd_asymbol_bfd(*symbols)) != NULL)
            && (!bfd_is_target_special_symbol(cur_bfd, *symbols))
            && contains_cgreen_spec((*symbols)->name)) {
            TestItem *test_item = create_test_item_from(cgreen_spec_start_of((*symbols)->name));
            if (verbose)
                printf("Discovered %s:%s (%s)\n", test_item->context_name, test_item->test_name,
                       test_item->specification_name);
            cgreen_vector_add(tests, test_item);
        }
    }
}

/* Read in the dynamic symbols.  */

static asymbol **get_symbols_table(bfd *abfd, long *symbol_count, bool verbose)
{
    long storage = bfd_get_dynamic_symtab_upper_bound(abfd);
    if (storage < 0) {
        if (!(bfd_get_file_flags(abfd) & DYNAMIC))
            if (verbose)
                printf("%s: not a dynamic object\n", bfd_get_filename(abfd));

        *symbol_count = 0;
        return NULL;
    }
    
    asymbol **symbols = (asymbol **) malloc(storage);
    if (symbols == NULL) {
        if (verbose)
            printf("malloc failed while retrieving symbols\n");
        *symbol_count = 0;
        return NULL;
    }

    *symbol_count = bfd_canonicalize_dynamic_symtab(abfd, symbols);
    if (*symbol_count < 0) {
        if (verbose)
            printf("%s: failed to get symbols count\n", bfd_get_filename(abfd));
        *symbol_count = 0;
        free(symbols);
        return NULL;
    }

  return symbols;
}

CgreenVector *discover_tests_in(const char *filename, bool verbose) {
    bfd* abfd = bfd_openr(filename, NULL);
    if (!abfd) {
        if (verbose)
            printf("%s: bfd_openr failed\n", filename);
        return NULL;
    }

    bfd_check_format(abfd, bfd_object);
    if ((bfd_get_file_flags(abfd) & HAS_SYMS) == 0) {
        if (verbose)
            printf("%s: incorrect format\n", filename);
        bfd_close(abfd);
        return NULL;
    }

    asymbol **symbols;
    long symbol_count;
    symbols = get_symbols_table(abfd, &symbol_count, verbose);

    if ((symbols == NULL) || (symbol_count <= 0)) {
        bfd_close(abfd);
        return NULL;
    }

    CgreenVector *tests = create_cgreen_vector((GenericDestructor)&destroy_test_item);
    add_all_tests_from(symbols, symbol_count, tests, verbose);
    return tests;
}
