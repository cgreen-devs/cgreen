typedef struct BasicStruct {
    int someValue;
} BasicStruct;

typedef BasicStruct* BasicStructPtr;

BasicStruct return_by_value(void);
BasicStructPtr return_pointer_to_struct(void);
BasicStruct* direct_return_pointer_to_struct(void);
