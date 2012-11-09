#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

char *read_paragraph(int (*read)(void *), void *stream);

static int stub_stream(void *stream) {
    return (int)mock(stream);
}

Ensure(reading_lines_from_empty_stream_gives_null) {
    always_expect(stub_stream, will_return(EOF));
    assert_that(read_paragraph(&stub_stream, NULL), is_null);
}

Ensure(one_character_stream_gives_one_character_line) {
    expect(stub_stream, will_return('a'));
    expect(stub_stream, will_return(EOF));
    char *line = read_paragraph(&stub_stream, NULL);
    assert_that(line, is_equal_to_string("a"));
    free(line);
}

Ensure(one_word_stream_gives_one_word_line) {
    expect(stub_stream, will_return('t'));
    expect(stub_stream, will_return('h'));
    expect(stub_stream, will_return('e'));
    always_expect(stub_stream, will_return(EOF));
    assert_that(read_paragraph(&stub_stream, NULL), is_equal_to_string("the"));
}

Ensure(drops_line_ending_from_word_and_stops) {
    expect(stub_stream, will_return('t'));
    expect(stub_stream, will_return('h'));
    expect(stub_stream, will_return('e'));
    expect(stub_stream, will_return('\n'));
    assert_that(read_paragraph(&stub_stream, NULL), is_equal_to_string("the"));
}

Ensure(single_line_ending_gives_empty_line) {
    expect(stub_stream, will_return('\n'));
    assert_that(read_paragraph(&stub_stream, NULL), is_equal_to_string(""));
}

static int reader(void *stream) {
    return (int)mock(stream);
}

static void writer(void *stream, char *paragraph) {
    mock(stream, paragraph);
}

Ensure(one_character_is_made_into_a_one_letter_paragraph) {
    expect(reader, will_return('a'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(no_line_endings_makes_one_paragraph) {
    expect(reader, will_return('a'));
    expect(reader, will_return(' '));
    expect(reader, will_return('b'));
    expect(reader, will_return(' '));
    expect(reader, will_return('c'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a b c")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(line_endings_generate_separate_paragraphs) {
    expect(reader, will_return('a'));
    expect(reader, will_return('\n'));
    expect(reader, will_return('b'));
    expect(reader, will_return('\n'));
    expect(reader, will_return('c'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a")));
    expect(writer, when(paragraph, is_equal_to_string("b")));
    expect(writer, when(paragraph, is_equal_to_string("c")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(resources_are_paired_with_the_functions) {
    expect(reader, when(stream, is_equal_to(1)), will_return('a'));
    always_expect(reader, when(stream, is_equal_to(1)), will_return(EOF));
    expect(writer, when(stream, is_equal_to(2)));
    by_paragraph(&reader, (void *)1, &writer, (void *)2);
}

Ensure(empty_paragraphs_are_ignored) {
    expect(reader, will_return('\n'));
    always_expect(reader, will_return(EOF));
    expect_never(writer);
    by_paragraph(&reader, NULL, &writer, NULL);
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test(suite, reading_lines_from_empty_stream_gives_null);
    add_test(suite, one_character_stream_gives_one_character_line);
    add_test(suite, one_word_stream_gives_one_word_line);
    add_test(suite, drops_line_ending_from_word_and_stops);
    add_test(suite, single_line_ending_gives_empty_line);
    add_test(suite, one_character_is_made_into_a_one_letter_paragraph);
    add_test(suite, no_line_endings_makes_one_paragraph);
    add_test(suite, line_endings_generate_separate_paragraphs);
    add_test(suite, resources_are_paired_with_the_functions);
    add_test(suite, empty_paragraphs_are_ignored);
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
