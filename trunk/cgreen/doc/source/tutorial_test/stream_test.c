#include <cgreen/cgreen.h>
#include <stdio.h>

char *read_paragraph(int (*read)(void *), void *stream);
void by_paragraph(int (*read)(void *), void *in, void (*write)(void *, char *), void *out);

static int stub_stream(void *stream) {
    return (int)stubbed_result();
}

static void reading_lines_from_empty_stream_gives_null() {
    always_return(stub_stream, EOF);
    assert_equal(read_paragraph(&stub_stream, NULL), NULL);
}

static void one_character_stream_gives_one_character_line() {
    will_return(stub_stream, 'a');
    will_return(stub_stream, EOF);
    char *line = read_paragraph(&stub_stream, NULL);
    assert_string_equal(line, "a");
    free(line);
}

static void one_word_stream_gives_one_word_line() {
    will_return(stub_stream, 't');
    will_return(stub_stream, 'h');
    will_return(stub_stream, 'e');
    always_return(stub_stream, EOF);
    assert_string_equal(read_paragraph(&stub_stream, NULL), "the");
}

static void drops_line_ending_from_word_and_stops() {
    will_return(stub_stream, 't');
    will_return(stub_stream, 'h');
    will_return(stub_stream, 'e');
    will_return(stub_stream, '\n');
    assert_string_equal(read_paragraph(&stub_stream, NULL), "the");
}

static void single_line_ending_gives_empty_line() {
    will_return(stub_stream, '\n');
    assert_string_equal(read_paragraph(&stub_stream, NULL), "");
}

static int reader(void *stream) {
    checked_integer(stream);
    return (int)stubbed_result();
}

static void writer(void *stream, char *paragraph) {
    checked_integer(stream);
    checked_string(paragraph);
}

static void one_character_is_made_into_a_one_letter_paragraph() {
    mock(reader, 'a', NULL);
    always_mock(reader, EOF, NULL);
    expect(writer, NULL, "a");
    by_paragraph(&reader, NULL, &writer, NULL);
}

static void no_line_endings_makes_one_paragraph() {
    mock(reader, 'a', NULL);
    mock(reader, ' ', NULL);
    mock(reader, 'b', NULL);
    mock(reader, ' ', NULL);
    mock(reader, 'c', NULL);
    always_mock(reader, EOF, NULL);
    expect(writer, NULL, "a b c");
    by_paragraph(&reader, NULL, &writer, NULL);
}

static void line_endings_generate_separate_paragraphs() {
    mock(reader, 'a', NULL);
    mock(reader, '\n', NULL);
    mock(reader, 'b', NULL);
    mock(reader, '\n', NULL);
    mock(reader, 'c', NULL);
    always_mock(reader, EOF, NULL);
    expect(writer, NULL, "a");
    expect(writer, NULL, "b");
    expect(writer, NULL, "c");
    by_paragraph(&reader, NULL, &writer, NULL);
}

static void resources_are_paired_with_the_functions() {
    mock(reader, 'a', (void *)1);
    always_mock(reader, EOF, (void *)1);
    expect(writer, (void *)2, "a");
    by_paragraph(&reader, (void *)1, &writer, (void *)2);
}

static void empty_paragraphs_are_ignored() {
    mock(reader, '\n', NULL);
    always_mock(reader, EOF, NULL);
    expect_never(writer);
    by_paragraph(&reader, NULL, &writer, NULL);
}

static TestSuite *stream_tests() {
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
    return suite;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
	add_suite(suite, stream_tests());
    return run_test_suite(suite, create_text_reporter());
}
