%s TEST_NAME
%s TEST_ADDER
%s TEST_LIST

int bracket_nesting = 0;

%%
add_tests\(                     { ECHO; BEGIN TEST_ADDER; }
[ \n]*Test[ ]+                  { ECHO; BEGIN TEST_NAME; }

<TEST_NAME>\(\)                 { ECHO; BEGIN 0; }
<TEST_NAME>[a-zA-Z0-9_]+        { printf("[%s]", yytext); }

<TEST_ADDER>\)                  { ECHO; BEGIN 0; }
<TEST_ADDER>[,)][ ]*            { printf(", "); BEGIN TEST_LIST; }
<TEST_ADDER>[^(),]+             { ECHO; }
                                
<TEST_LIST>[^)]+\)              {
                                    printf("%s", "[&my_test])");
                                    BEGIN 0;
                                }
%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    yylex();
    return 0;
}
