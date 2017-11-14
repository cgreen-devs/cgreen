#ifndef TEST_ITEM_H
#define TEST_ITEM_H

/* The name of a test is either a named mangled from the name of the
   context, if any, and the actual test name.  Names (or patterns) on
   the command line is formatted as a symbolic name composed of the
   context name, a colon and the test name. The variable naming below
   is trying to be clear about which type of name it is. */

typedef struct test_item {
    const char *specification_name;
    const char *context_name;
    const char *test_name;
} TestItem;

extern TestItem *create_test_item_from(const char *specification_name);
extern void destroy_test_item(TestItem *item);

#endif
