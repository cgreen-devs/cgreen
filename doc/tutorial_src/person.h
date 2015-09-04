typedef struct _Person {
    char *name;
} Person;

Person *create_person();
Person *find_person_by_name(const char *);
void set_person_name(Person *, const char *);
char *get_person_name(Person *);
int save_person(Person *);
