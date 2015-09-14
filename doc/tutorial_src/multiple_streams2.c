#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

void do_stuff(void *stream1, void *stream2) {}

Describe(Streams);
BeforeEach(Streams) {}
AfterEach(Streams) {}

static int first_stream_stub(void *stream) {
    return (int)mock(stream);
}

static int second_stream_stub(void *stream) {
    return (int)mock(stream);
}

Ensure(Streams, good_test) {
    expect(first_stream_stub, will_return('a'));
    expect(second_stream_stub, will_return('a'));
    do_stuff(&first_stream_stub, &second_stream_stub);
}
