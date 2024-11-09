#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

void do_stuff(void *stream1, void *stream2) {}

Describe(Streams);
BeforeEach(Streams) {}
AfterEach(Streams) {}

static int stream_stub(void *stream) {
    return (int)mock(stream);
}

Ensure(Streams, bad_test) {
    expect(stream_stub, will_return('a'));
    do_stuff(&stream_stub, &stream_stub);
}
