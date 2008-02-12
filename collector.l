%s TEST_NAME
%s TEST_ADDER
%s TEST_LIST

    #include "vector.h"
    #include <string.h>
    #include <stdio.h>
    
    void as_function_list(CgreenVector *vector);
    void destroy_string(void *string);
    
    CgreenVector *tests;
%%
add_tests\(                     { ECHO; BEGIN TEST_ADDER; }
[ \n]*Ensure[ ]+                { ECHO; BEGIN TEST_NAME; }

<TEST_NAME>\(\)                 { ECHO; BEGIN 0; }
<TEST_NAME>[a-zA-Z0-9_]+        {
                                    cgreen_vector_add(tests, strdup(yytext));
                                    ECHO;
                                }

<TEST_ADDER>\)                  { ECHO; BEGIN 0; }
<TEST_ADDER>[,)][ ]*            { printf(", "); BEGIN TEST_LIST; }
<TEST_ADDER>[^(),]+             { ECHO; }
                                
<TEST_LIST>[^)]+\)              {
                                    as_function_list(tests);
                                    printf(")");
                                    destroy_cgreen_vector(tests);
                                    tests = create_cgreen_vector(&destroy_string);
                                    BEGIN 0;
                                }
%%
    int yywrap() {
        return 1;
    }
    
    void destroy_string(void *string) {
        free((char *)string);
    }
    
    void as_function_list(CgreenVector *vector) {
        int i;
        for (i = 0; i < cgreen_vector_size(vector); i++) {
            printf("%s&%s", (i == 0 ? "" : ", "), cgreen_vector_get(vector, i));
        }
    }
    
    int main(int argc, char **argv) {
        tests = create_cgreen_vector(&destroy_string);
        yylex();
        destroy_cgreen_vector(tests);
        return 0;
    }
