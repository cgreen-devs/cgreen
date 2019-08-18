typedef struct Struct1 {
    int someValue;
} Struct1;

typedef Struct1* Struct1Ptr;

Struct1 return_struct1_by_value(int i);
Struct1Ptr return_pointer_to_struct1(char string[]);
Struct1* direct_return_pointer_to_struct1(void);

typedef struct Struct2 {
    int someValue;
} Struct2;

typedef Struct2* Struct2Ptr;

Struct2 return_struct2_by_value(int i);
Struct2Ptr return_pointer_to_struct2(char string[]);
Struct2* direct_return_pointer_to_struct2(void);

extern void return_nothing(void);
extern int return_int(int ints[]);
