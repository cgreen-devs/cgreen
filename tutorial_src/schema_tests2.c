#include <cgreen/cgreen.h>
#include <stdlib.h>
#include <mysql.h>
#include "person.h"

static void create_schema() {
    MYSQL *connection = mysql_init(NULL);
    mysql_real_connect(connection, "localhost", "me", "secret", "test", 0, NULL, 0);
    mysql_query(connection, "create table people (name, varchar(255) unique)");
    mysql_close(connection);
}

static void drop_schema() {
    MYSQL *connection = mysql_init(NULL);
    mysql_real_connect(connection, "localhost", "me", "secret", "test", 0, NULL, 0);
    mysql_query(connection, "drop table people");
    mysql_close(connection);
}

Describe(Person);
BeforeEach(Person) { create_schema(); }
AfterEach(Person) { drop_schema(); }

Ensure(Person, can_add_person_to_database) {
    Person *person = create_person();
    set_person_name(person, "Fred");
    save_person(person);
    Person *found = find_person_by_name("Fred");
    assert_that(get_person_name(found), is_equal_to_string("Fred"));
}

Ensure(Person, cannot_add_duplicate_person) {
    Person *person = create_person();
    set_person_name(person, "Fred");
    assert_that(save_person(person), is_true);
    Person *duplicate = create_person();
    set_person_name(duplicate, "Fred");
    assert_that(save_person(duplicate), is_false);
}

TestSuite *person_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Person, can_add_person_to_database);
    add_test_with_context(suite, Person, cannot_add_duplicate_person);
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(person_tests(), create_text_reporter());
}
