# Cgreen Cheat Sheet

## Pre-ample

    #include <cgreen/cgreen.h>
    #include <cgreen/mocks.h>

    Describe( <sut> );
    BeforeEach( <sut> ) {}
    AfterEach( <sut> ) {}

## A test

    Ensure( <sut>, <test_name> ) {
        assert_that( <expression> );
        assert_that( <expression>, <constraint> );
        assert_that_double(...);
    }

## Constraints

    is_true
    is_false
    is_null
    is_non_null
    is_equal_to( <value> )
    is_equal_to_hex( <value> )
    is_not_equal_to( <value> )
    is_greater_than( <value> )
    is_less_than( <value> )

### Structs and general data

    is_equal_to_contents_of( <pointer>, <size> )
    is_not_equal_to_contents_of( <pointer>, <size> )

### Strings

    is_equal_to_string( <value> )
    is_not_equal_to_string( <value> )
    contains_string( <value> )
    does_not_contain_string( <value> )
    begins_with_string( <value> )
    does_not_begin_with_string( <value> )
    ends_with_string( <value> )
    does_not_end_with_string( <value> )

### Doubles

    is_equal_to_double( <value> )
    is_not_equal_to_double( <value> )
    is_less_than_double( <value> )
    is_greater_than_double( <value> )

    significant_figures_for_assert_double_are( <figures> )

## Mocks

    <qualifiers> <function_name> ( <parameters> ) {
        mock( <parameters> );
        mock( box_double( <parameter> ) );
        return ( <type> ) mock( <parameters> );
        return mock_double( <parameters> );
    }

    Ensure( <sut> ) {
        expect(<function> {, when(<parameter>, <constraint> ) }
                          [, <returns> ]
                          [, times( <n> ) ] );
        always_expect(...);
        never_expect(...);
    }

    cgreen_mocks_are( strict_mocks | loose_mocks | learning_mocks );

### Returns

    will_return( <value> )
    will_return_double( <value> )
    will_return_by(value( <value>, <size> )
    with_side_effect( <function>, <pointer_to_data> )
    will_set_contents_of_parameter( <parameter_name>, <value>, <size> )

## C++

    namespace cgreen;
    assert_throws( <exception>, <expression>);
