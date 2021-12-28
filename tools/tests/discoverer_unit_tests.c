#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "discoverer.h"
#include "test_item.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bfd_adapter.h"
#include "bfd_adapter.mocks"


int printf_unittesting(const char *fmt, ...) {
    return (int) mock(fmt);
}

Describe(Discoverer);
BeforeEach(Discoverer) {}
AfterEach(Discoverer) {}


static bool verbose = true;


/* Helper functions */
static void expect_open_file(const char *filename, void *result) {
    expect(bfd_adapter_openr,
           when(filename, is_equal_to_string(filename)),
           when(target, is_null),
           will_return(result));
}

static void given_a_dynamic_file_with_symbols(const char *filename, bfd *expect_abfd) {
    expect_open_file(filename, expect_abfd);
    expect(bfd_adapter_check_format,
           when(abfd, is_equal_to(expect_abfd)),
           when(format, is_equal_to(bfd_object)));
    always_expect(bfd_adapter_get_file_flags,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(HAS_SYMS|DYNAMIC));
}

static void given_a_non_dynamic_file_with_symbols(const char *filename, bfd *expect_abfd) {
    expect_open_file(filename, expect_abfd);
    expect(bfd_adapter_check_format,
           when(abfd, is_equal_to(expect_abfd)),
           when(format, is_equal_to(bfd_object)));
    always_expect(bfd_adapter_get_file_flags,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(HAS_SYMS));
}

static void given_a_file_with_two_lines(const char *filename, const char *line1, const char *line2) {
    static bfd *expect_abfd = (bfd *) 1;
    static long expect_storage = 1;
    static asymbol symbols[2];
    symbols[0].name = line1;
    symbols[1].name = line2;
    static asymbol *symbols_ptr[2] = { &symbols[0], &symbols[1] };
    static asymbol **expect_symbols = (asymbol **) &symbols_ptr;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(2));
    expect(bfd_adapter_asymbol_bfd,
           when(sy, is_equal_to(symbols_ptr[0])),
           will_return(expect_abfd));
    expect(bfd_adapter_is_target_special_symbol,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(symbols_ptr[0])),
           will_return(false));
    expect(bfd_adapter_asymbol_bfd,
           when(sy, is_equal_to(symbols_ptr[1])),
           will_return(expect_abfd));
    expect(bfd_adapter_is_target_special_symbol,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(symbols_ptr[1])),
           will_return(false));
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));
}

static void given_a_file_with_one_line(const char *filename, const char *line) {
    static bfd *expect_abfd = (bfd *) 1;
    static long expect_storage = 1;
    static asymbol symbols[1];
    symbols[0].name = line;
    static asymbol *symbols_ptr = symbols;
    static asymbol **expect_symbols = (asymbol **) &symbols_ptr;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(1));
    expect(bfd_adapter_asymbol_bfd,
           when(sy, is_equal_to(symbols_ptr)),
           will_return(expect_abfd));
    expect(bfd_adapter_is_target_special_symbol,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(symbols_ptr)),
           will_return(false));
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_in_non_existing_file) {
    expect_open_file("non-existing-file", NULL);
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("non-existing-file", verbose);

    assert_that(tests, is_null);
}

/*======================================================================*/
/* Any input file that has no object: text file, pdf, etc. */
Ensure(Discoverer, should_find_no_tests_if_file_has_no_symbol) {
    const char *filename = "no-symbol";
    bfd *expect_abfd = (bfd *)1;

    expect_open_file(filename, expect_abfd);
    expect(bfd_adapter_check_format,
           when(abfd, is_equal_to(expect_abfd)),
           when(format, is_equal_to(bfd_object)));
    expect(bfd_adapter_get_file_flags,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(0));
    expect(printf_unittesting);
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
/* Any input file that has object but has no symbols */
Ensure(Discoverer, should_find_no_tests_if_file_has_no_dynamic_symbol) {
    const char *filename = "no-dynamic-symbol";
    bfd *expect_abfd = (bfd *)1;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(-1));
    expect(printf_unittesting);
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

Ensure(Discoverer, should_find_no_tests_if_non_dynamic_file_has_no_symbol) {
    const char *filename = "no-dynamic-symbol";
    bfd *expect_abfd = (bfd *)1;

    given_a_non_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(-1));
    expect(printf_unittesting);
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_if_bdf_alloc_fail) {
    const char *filename = "valid-file";
    bfd *expect_abfd = (bfd *)1;
    long expect_storage = 1;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(NULL));
    expect(printf_unittesting);
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_if_bfd_canonicalize_dynamic_symtab_fails) {
    const char *filename = "valid-file";
    bfd *expect_abfd = (bfd *) 1;
    long expect_storage = 1;
    asymbol **expect_symbols = (asymbol **) 2;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(-1));
    expect(printf_unittesting);
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_if_get_symbols_table_return_zero_symbol) {
    const char *filename = "valid-file";
    bfd *expect_abfd = (bfd *) 1;
    long expect_storage = 1;
    asymbol **expect_symbols = (asymbol **) 2;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(0));
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_if_bfd_asymbol_bfd_fails) {
    const char *filename = "valid-file";
    bfd *expect_abfd = (bfd *) 1;
    long expect_storage = 1;
    asymbol symbols[1];
    symbols[0].name = "aline";
    asymbol *symbols_ptr = symbols;
    asymbol **expect_symbols = (asymbol **) &symbols_ptr;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(1));
    expect(bfd_adapter_asymbol_bfd,
           when(sy, is_equal_to(symbols_ptr)),
           will_return(NULL));
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_if_bfd_is_target_special_symbol_returns_true) {
    const char *filename = "valid-file";
    bfd *expect_abfd = (bfd *) 1;
    long expect_storage = 1;
    asymbol symbols[1];
    symbols[0].name = "aline";
    asymbol *symbols_ptr = symbols;
    asymbol **expect_symbols = (asymbol **) &symbols_ptr;

    given_a_dynamic_file_with_symbols(filename, expect_abfd);

    expect(bfd_adapter_get_dynamic_symtab_upper_bound,
           when(abfd, is_equal_to(expect_abfd)),
           will_return(expect_storage));
    expect(bfd_adapter_alloc,
           when(abfd, is_equal_to(expect_abfd)),
           when(wanted, is_equal_to(expect_storage)),
           will_return(expect_symbols));
    expect(bfd_adapter_canonicalize_dynamic_symtab,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(expect_symbols)),
           will_return(1));
    expect(bfd_adapter_asymbol_bfd,
           when(sy, is_equal_to(symbols_ptr)),
           will_return(expect_abfd));
    expect(bfd_adapter_is_target_special_symbol,
           when(abfd, is_equal_to(expect_abfd)),
           when(symbols, is_equal_to(symbols_ptr)),
           will_return(true));
    expect(bfd_adapter_close,
           when(abfd, is_equal_to(expect_abfd)));

    CgreenVector *tests = discover_tests_in(filename, verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}
/*======================================================================*/
Ensure(Discoverer, should_find_one_test_in_file_with_one_line_containing_testname_pattern) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 D CgreenSpec__A_context__a_test__");
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}

/*======================================================================*/
Ensure(Discoverer, should_find_two_test_in_two_line_file_with_two_lines_containing_testname_pattern) {
    given_a_file_with_two_lines("some-file",
                                "0000000000202160 D CgreenSpec__Context1__test1__",
                                "0000000000202160 D CgreenSpec__Context2__test2__");
    expect(printf_unittesting);
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(2));
}

/*======================================================================*/
Ensure(Discoverer, should_find_one_test_in_two_line_file_with_one_line_containing_testname_pattern) {
    given_a_file_with_two_lines("some-file",
                                "0000000000202160 D CgreenSpec__Discoverer__test1__",
                                "0000000000202160 D ID");
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}

/*======================================================================*/
Ensure(Discoverer, should_return_valid_test_item_for_a_line_containing_testname_pattern) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 D CgreenSpec__Context1__test_1__");
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));

    TestItem *test_item = (TestItem*)cgreen_vector_get(tests, 0);
    assert_that(test_item->specification_name, is_equal_to_string("CgreenSpec__Context1__test_1__"));
    assert_that(test_item->context_name, is_equal_to_string("Context1"));
    assert_that(test_item->test_name, is_equal_to_string("test_1"));
}

/*======================================================================*/
Ensure(Discoverer, can_handle_truncated_testnames) {
    given_a_file_with_one_line("some-file", "0000000000202160 D CgreenSpec__Context1__a");
    expect(printf_unittesting);

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    TestItem *test_item = (TestItem*)cgreen_vector_get(tests, 0);
    assert_that(test_item->test_name, is_equal_to_string("a"));
}
