#include "discoverer.h"

#include "bfd_adapter.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <cgreen/internal/unit_implementation.h>

#include "../src/utils.h"

#include "test_item.h"

#ifdef UNITTESTING
int printf_unittesting(const char *fmt, ...);
#define printf printf_unittesting
#endif

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
            && ((cur_bfd = bfd_adapter_asymbol_bfd(*symbols)) != NULL)
            && (!bfd_adapter_is_target_special_symbol(cur_bfd, *symbols))
            && contains_cgreen_spec((*symbols)->name)) {
            TestItem *test_item = create_test_item_from(cgreen_spec_start_of((*symbols)->name));
            if (verbose)
                printf("Discovered %s:%s (%s)\n", test_item->context_name, test_item->test_name,
                       test_item->specification_name);
            cgreen_vector_add(tests, test_item);
        }
    }
}

/* Read in the symbols.  */
static asymbol **get_symbols_table(bfd *abfd, long *symbol_count, bool verbose, const char *filename)
{
    bool dynamic = (bfd_adapter_get_file_flags(abfd) & DYNAMIC) != 0;
    long storage;

    if (dynamic)
        storage = bfd_adapter_get_dynamic_symtab_upper_bound(abfd);
    else
        storage = bfd_adapter_get_symtab_upper_bound(abfd);
    if (storage <= 0) {
        if (verbose) {
            if (storage < 0)
                printf("%s: bfd_get_symtab_upper_bounds returned %ld\n", filename, storage);
            else
                printf("%s: no symbols\n", filename);
        }
        *symbol_count = 0;
        return NULL;
    }

    asymbol **symbols = (asymbol **) bfd_adapter_alloc(abfd, storage);
    if (symbols == NULL) {
        if (verbose)
            printf("%s: bfd_alloc failed while retrieving symbols\n", filename);
        *symbol_count = 0;
        return NULL;
    }

    if (dynamic)
        *symbol_count = bfd_adapter_canonicalize_dynamic_symtab(abfd, symbols);
    else
        *symbol_count = bfd_adapter_canonicalize_symtab(abfd, symbols);
    if (*symbol_count < 0) {
        if (verbose)
            printf("%s: failed to get symbols count\n", filename);
        *symbol_count = 0;
        return NULL;
    }

    return symbols;
}

CgreenVector *discover_tests_in(const char *filename, bool verbose) {
    bfd* abfd = bfd_adapter_openr(filename, NULL);
    if (!abfd) {
        if (verbose)
            printf("%s: bfd_adapter_openr failed\n", filename);
        return NULL;
    }

    bfd_adapter_check_format(abfd, bfd_object);
    if ((bfd_adapter_get_file_flags(abfd) & HAS_SYMS) == 0) {
        if (verbose)
            printf("%s: file flags indicate no symbols\n", filename);
        bfd_adapter_close(abfd);
        return NULL;
    }

    asymbol **symbols;
    long symbol_count;
    symbols = get_symbols_table(abfd, &symbol_count, verbose, filename);

    if ((symbols == NULL) || (symbol_count <= 0)) {
        bfd_adapter_close(abfd);
        return NULL;
    }

    CgreenVector *tests = create_cgreen_vector((GenericDestructor)&destroy_test_item);
    add_all_tests_from(symbols, symbol_count, tests, verbose);

    bfd_adapter_close(abfd);

    return tests;
}
