#include <stdio.h>

typedef struct _Person {
    char *name;
} Person;

Person *create_person() {return NULL;}
Person *find_person_by_name(const char *name) {return NULL;}
void set_person_name(Person *person, const char *name) {}
char *get_person_name(Person *person) {return NULL;}
int save_person(Person *person) { return 1;}
