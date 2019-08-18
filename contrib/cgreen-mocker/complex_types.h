typedef struct BasicStruct {
    int someValue;
} BasicStruct;

typedef BasicStruct* BasicStructPtr;

BasicStruct return_struct_by_value(int i);
BasicStructPtr return_pointer_to_struct(char string[]);
BasicStruct* direct_return_pointer_to_struct(void);
