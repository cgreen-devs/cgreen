%s TEST_NAME
%s TEST_ADDER
%s TEST_LIST
%s IN_BRACKETS
%s IN_STRING

    #include "collector_test_list.h"
    #include <string.h>
    #include <stdio.h>
%%
    int bracket_nesting = 0;

add_tests\(                     { ECHO; BEGIN TEST_ADDER; }
[ \n]*Ensure[ ]+                { ECHO; BEGIN TEST_NAME; }

<TEST_NAME>\(\)                 { ECHO; BEGIN 0; }
<TEST_NAME>[a-zA-Z0-9_]+        {
                                    add_to_test_list(strdup(yytext));
                                    ECHO;
                                }

<TEST_ADDER>\(                  { ECHO; BEGIN IN_BRACKETS; }
<TEST_ADDER>[),][ ]*            { printf(", "); BEGIN TEST_LIST; }
<TEST_ADDER>[^(),]+             { ECHO; }

<IN_BRACKETS>\)                 {
                                    ECHO;
                                    if (! bracket_nesting) {
                                        printf(", ");
                                        BEGIN TEST_LIST;
                                    } else {
                                        bracket_nesting--;
                                    }
                                }
<IN_BRACKETS>\(                 { ECHO; bracket_nesting++; }
<IN_BRACKETS>\"                 { ECHO; BEGIN IN_STRING; }
<IN_BRACKETS>[^()"]+            { ECHO; }

<IN_STRING>\\\"                 { ECHO; }
<IN_STRING>\"                   { ECHO; BEGIN IN_BRACKETS; }
<IN_STRING>[^"]+                { ECHO; }
                                
<TEST_LIST>[^)]*\)              {
                                    print_test_list();
                                    printf(")");
                                    reset_test_list();
                                    BEGIN 0;
                                }
%%
    int yywrap() {
        return 1;
    }
    
    int main(int argc, char **argv) {
        create_test_list();
        yylex();
        destroy_test_list();
        return 0;
    }
