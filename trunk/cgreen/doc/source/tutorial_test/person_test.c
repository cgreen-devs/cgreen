#include <cgreen/cgreen.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "person.h"

static MYSQL *connection;

static void create_schema() {
    mysql_query(connection, "create table people (name, varchar(255) unique)");
}

static void drop_schema() {
    mysql_query(connection, "drop table people");
}

static void can_add_person_to_database() {
    Person *person = create_person();
    set_person_name(person, "Fred");
    save_person(person);
    Person *found = find_person_by_name("Fred");
    assert_string_equal(get_person_name(person), "Fred", NULL);
}

static void cannot_add_duplicate_person() {
    Person *person = create_person();
    set_person_name(person, "Fred");
    assert_true(save_person(person), NULL);
    Person *duplicate = create_person();
    set_person_name(duplicate, "Fred");
    assert_false(save_person(duplicate), NULL);
}

void open_connection() {
    connection = mysql_init(NULL);
    mysql_real_connect(connection, "localhost", "me", "secret", "test", 0, NULL, 0);
}

void close_connection() {
    mysql_close(connection);
}

TestSuite *person_tests() {
    TestSuite *suite = create_test_suite();
    setup(suite, create_schema);
    teardown(suite, drop_schema);
    add_test(suite, can_add_person_to_database);
    add_test(suite, cannot_add_duplicate_person);

    TestSuite *fixture = create_named_test_suite("Mysql");
    add_suite(fixture, suite);
    setup(fixture, open_connection);
    teardown(fixture, close_connection);
    return fixture;
}

int main(int argc, char **argv) {
    return run_test_suite(person_tests(), create_text_reporter());
}
