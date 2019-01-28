#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

static int reader(void *stream) {
    return (int)mock(stream);
}

static void writer(void *stream, char *paragraph) {
    mock(stream, paragraph);
}

char *read_paragraph(int (*read)(void *), void *stream);

void by_paragraph(int (*read)(void *), void *in, void (*write)(void *, char *), void *out) {
    while (1) {
        char *line = read_paragraph(read, in);
        if ((line == NULL) || (strlen(line) == 0)) {
            return;
        }
        (*write)(out, line);
        free(line);
    }
}

static int stub_stream(void *stream) {
    return (int)mock(stream);
}

static void update_counter(void * counter) {
    *(int*)counter = *(int*)counter + 1;
}

Ensure(using_side_effect) {
    int number_of_times_reader_was_called = 0;
    expect(reader, will_return('\n'));
    always_expect(reader,
                  will_return(EOF),
                  with_side_effect(&update_counter,
                                  &number_of_times_reader_was_called));
    expect_never(writer);
    by_paragraph(&reader, NULL, &writer, NULL);

    assert_that(number_of_times_reader_was_called, is_equal_to(1));
}
