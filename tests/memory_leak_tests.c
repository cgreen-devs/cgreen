#include <cgreen/cgreen.h>
#include <cgreen/messaging.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/cgreen/memory.h"

Describe(MemoryLeak)
BeforeEach(MemoryLeak) {}
AfterEach(MemoryLeak) {}

Ensure(MemoryLeak, free_null_pointer) {
    free(NULL);
}

Ensure(MemoryLeak, alloc_and_free_memory) {
    int* new_int = malloc(sizeof(int));
    free(new_int);
    check_memory_leak(get_test_reporter());
    assert_that(true, is_true);
}

Ensure(MemoryLeak, multiple_allocations_and_free_of_memory_in_different_order_works) {
    int* new_int = malloc(sizeof(int));
    int* new_int1 = malloc(sizeof(int));
    int* new_int2 = malloc(sizeof(int));

    free(new_int);

    int* new_int3 = malloc(sizeof(int));

    free(new_int2);
    free(new_int1);
    free(new_int3);

    check_memory_leak(get_test_reporter());
    assert_that(true, is_true);
}

Ensure(MemoryLeak, free_unallocate_memory) {
    free((void*)1);

    check_memory_leak(get_test_reporter());
}

Ensure(MemoryLeak, fail_test_when_memory_is_leaked) {
    malloc(sizeof(int));

    check_memory_leak(get_test_reporter());
}

Ensure(MemoryLeak, use_realloc_maintain_data_in_string) {
    char * new_pointer = malloc(sizeof(char) * 5);
    strcpy(new_pointer, "123");

    char* new_string = realloc(new_pointer, sizeof(char) * 10);
    assert_that(new_string, is_equal_to_string("123"));

    free(new_string);
}

Ensure(MemoryLeak, use_realloc_to_increase_the_size_of_a_pointer) {
    char * new_pointer = malloc(sizeof(char) * 5);

    char* new_string = realloc(new_pointer, sizeof(char) * 10);
    strcpy(new_string, "123456789");
    free(new_string);
    check_memory_leak(get_test_reporter());
}

Ensure(MemoryLeak, use_realloc_to_decrease_the_size_of_a_pointer) {
    char * new_pointer = malloc(sizeof(char) * 10);

    char* new_string = realloc(new_pointer, sizeof(char) * 5);
    strcpy(new_string, "1234");

    free(new_string);
    check_memory_leak(get_test_reporter());
}

Ensure(MemoryLeak, use_calloc_to_allocate_an_array) {
    int * array = calloc(5, sizeof(int));

    array[0] = 1;
    array[1] = 1;
    array[2] = 1;
    array[3] = 1;
    array[4] = 1;

    free(array);
    check_memory_leak(get_test_reporter());
}
