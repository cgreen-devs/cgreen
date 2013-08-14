#ifndef CGREEN_CPP_ASSERTIONS_H
#define CGREEN_CPP_ASSERTIONS_H

#define assert_throws(exceptionType, expr)                              \
    try {                                                               \
        expr;                                                           \
        fail_test("Expected [" #expr "] to throw [" #exceptionType "]"); \
    } catch (const exceptionType& ex) {                                 \
        pass_test();                                                    \
    } catch (const exceptionType* ex) {					\
	pass_test();							\
    }

#endif
