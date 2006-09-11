#include "../cgreen.h"
#include "../messaging.h"
#include <stdlib.h>
#include <sys/msg.h>

void single_message_should_be_received() {
    MessageQueue *queue = start_messaging(2);
    send_message(queue, 99);
    assert_equal(receive_message(queue), 99, NULL);
    msgctl(queue->queue, IPC_RMID, NULL);
}

TestSuite *messaging_tests() {
    TestSuite *suite = create_test_suite();
    add_unit_test(suite, single_message_should_be_received);
    return suite;
}
