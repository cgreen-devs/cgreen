#include <stdlib.h>
#include <string.h>
#include <include/cgreen/reporter.h>
#include <stdio.h>
#include "include/cgreen/memory.h"

typedef struct MemoryEntry_ {
    void * memory_location;
    size_t size;
    const char* filename;
    int line_number;
} MemoryEntry;


static MemoryEntry ** allocation_memory = NULL;
static int number_of_memory_entries = 0;


static int find_memory_position(void * pointer);
static void move_memory_back(int position_to_start_on);
static MemoryEntry * get_memory_entry(void * pointer);

#undef malloc
static void create_allocation_memory(void) {
    allocation_memory = malloc(sizeof(MemoryEntry)*100);
}
#define malloc cgreen_malloc

#undef malloc
void* cgreen_malloc(size_t size, const char * filename, int line) {
    if(allocation_memory == NULL)
        create_allocation_memory();

    void * new_memory_pointer = malloc(size);
    MemoryEntry * new_memory_entry = malloc(sizeof(MemoryEntry));
    new_memory_entry->memory_location = new_memory_pointer;
    new_memory_entry->size = size;
    new_memory_entry->filename = filename;
    new_memory_entry->line_number = line;
    allocation_memory[number_of_memory_entries] = new_memory_entry;
    number_of_memory_entries++;

    return new_memory_pointer;
}
#define malloc cgreen_malloc

void* cgreen_calloc(size_t num_members, size_t size, const char * filename, int line) {
    void * allocated_memory = cgreen_malloc(num_members * size, filename, line);
    if (allocated_memory)
        memset(allocated_memory, 0, num_members * size);
    return allocated_memory;
}

void* cgreen_realloc(void* pointer, size_t size, const char * filename, int line) {
    if (pointer == NULL)
        return cgreen_malloc(size, filename, line);

    if (size == 0) {
        cgreen_free(pointer, filename, line);
        return NULL;
    }

    MemoryEntry * memory_position = get_memory_entry(pointer);

    void * new_pointer = cgreen_malloc(size, filename, line);

    size_t actual_size = size;
    if (size > memory_position->size)
        actual_size = memory_position->size;

    memcpy(new_pointer, pointer, actual_size);

    cgreen_free(pointer, filename, line);

    return new_pointer;
}


#undef free
void cgreen_free(void * pointer, const char * filename, const int line) {
    va_list null_arguments;
    memset(&null_arguments, 0, sizeof(null_arguments));
    if (pointer == NULL) {

        TestReporter *reporter = get_test_reporter();
        reporter->show_fail(reporter, filename, line, "Trying to free memory using a NULL pointer", null_arguments);
        send_reporter_exception_notification(reporter);
        return;
    }

    int memory_position = find_memory_position(pointer);

    if (memory_position == -1) {
        char message[100];
        sprintf(message, "Trying to free memory [%p] that was not previously allocated", pointer);

        TestReporter *reporter = get_test_reporter();
        reporter->show_fail(reporter, filename, line, message, null_arguments);
        send_reporter_exception_notification(reporter);
        return;
    }

    free(allocation_memory[memory_position]);
    move_memory_back(memory_position);
    number_of_memory_entries--;
}
#define free cgreen_free

void check_memory_leak(TestReporter *reporter) {
    if (number_of_memory_entries > 0) {
        for( int i = 0 ; i < number_of_memory_entries ; i++) {
            MemoryEntry * memoryEntry = allocation_memory[i];
            (reporter->assert_true)(
                    reporter,
                    memoryEntry->filename,
                    memoryEntry->line_number,
                    0,
                    "Memory allocated [%p] was not freed [%s:%d]",
                    memoryEntry->memory_location,
                    memoryEntry->filename,
                    memoryEntry->line_number
            );
        }
    }
}

int find_memory_position(void * pointer) {
    for( int i = 0 ; i < number_of_memory_entries ; i++) {
        MemoryEntry * memoryEntry = allocation_memory[i];
        if(memoryEntry && memoryEntry->memory_location == pointer)
            return i;
    }

    return -1;
}

MemoryEntry * get_memory_entry(void * pointer) {
    int position = find_memory_position(pointer);

    if (position == -1)
        return NULL;

    return allocation_memory[position];
}

void move_memory_back(int position_to_start_on) {
    for(int i = position_to_start_on ; i < number_of_memory_entries - 1 ; i++) {
        allocation_memory[i] = allocation_memory[i+1];
    }
}