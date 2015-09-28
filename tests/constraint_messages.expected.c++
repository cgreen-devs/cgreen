Running "constraint_messages" (27 tests)...
constraint_messages.c:117: Failure: FailureMessage -> for_always_followed_by_expectation 
	Mocked function [some_mock] already has an expectation that it will always be called a certain way; any expectations declared after an always expectation are invalid

constraint_messages.c:92: Failure: FailureMessage -> for_assert_that 
	Expected [0 == 1] to [be true]

constraint_messages.c:80: Failure: FailureMessage -> for_begins_with_string 
	Expected [does_not_begin_with_fourty_five] to [begin with string] [fourty_five]
		actual value:			["this string does not begin with fortyfive"]
		expected to begin with:		["fourtyfive"]

constraint_messages.c:70: Failure: FailureMessage -> for_contains_string 
	Expected [not_containing_fourty_five] to [contain string] [fourty_five]
		actual value:			["this text is thirtythree"]
		expected to contain:		["fortyfive"]

constraint_messages.c:75: Failure: FailureMessage -> for_does_not_contain_string 
	Expected [contains_fourty_five] to [not contain string] [fourty_five]
		actual value:			["this string is fourtyfive"]
		expected to not contain:	["fourtyfive"]

constraint_messages.c:84: Failure: FailureMessage -> for_equal_to_double 
	Expected [0] to [equal double] [1] within [8] significant figures
		actual value:	0.000000
		expected value:	1.000000

constraint_messages.c:145: Exception: FailureMessage -> for_incorrect_assert_throws 
	an exception was thrown during test: [something else]

constraint_messages.c:30: Failure: FailureMessage -> for_is_equal_to 
	Expected [fourty_five] to [equal] [thirty_three]
		actual value:			[45]
		expected value:			[33]

constraint_messages.c:50: Failure: FailureMessage -> for_is_equal_to_contents_of 
	Expected [thirty_three] to [equal contents of] [fourty_five]
		at offset:			[9]

constraint_messages.c:60: Failure: FailureMessage -> for_is_equal_to_string 
	Expected [thirty_three] to [equal string] [fourty_five]
		actual value:			["this string is thirtythree"]
		expected to equal:		["this string is fortyfive"]

constraint_messages.c:40: Failure: FailureMessage -> for_is_greater_than 
	Expected [thirty_three] to [be greater than] [fourty_five]
		actual value:			[33]
		expected to be greater than:	[45]

constraint_messages.c:45: Failure: FailureMessage -> for_is_less_than 
	Expected [fourty_five] to [be less than] [thirty_three]
		actual value:			[45]
		expected to be less than:	[33]

constraint_messages.c:25: Failure: FailureMessage -> for_is_non_null 
	Expected [pointer] to [be non null]

constraint_messages.c:35: Failure: FailureMessage -> for_is_not_equal_to 
	Expected [should_not_be_fourty_five] to [not equal] [forty_five]
		actual value:			[45]

constraint_messages.c:55: Failure: FailureMessage -> for_is_not_equal_to_contents_of 
	Expected [fourty_five_and_up] to [not equal contents of] [another_fourty_five_and_up]
		at offset:			[-1]

constraint_messages.c:65: Failure: FailureMessage -> for_is_not_equal_to_string 
	Expected [another_fourty_five] to [not equal string] [fourty_five]
		actual value:			["this string is fourtyfive"]

constraint_messages.c:20: Failure: FailureMessage -> for_is_null 
	Expected [pointer] to [be null]

constraint_messages.c:96: Failure: FailureMessage -> for_mock_called_more_times_than_expected 
	Mocked function [some_mock] was called too many times

constraint_messages.c:110: Failure: FailureMessage -> for_mock_called_with_unexpected_parameter_value 
	Expected [[parameter] parameter in [some_mock]] to [equal] [1]
		actual value:			[0]
		expected value:			[1]

constraint_messages.c:96: Failure: FailureMessage -> for_mock_called_without_expectation 
	Mocked function [some_mock] did not have an expectation that it would be called

constraint_messages.c:126: Failure: FailureMessage -> for_mock_parameter_name_not_matching_constraint_parameter_name 
	Mocked function [some_mock] did not define a parameter named [PARAMETER]. Did you misspell it in the expectation or forget it in the mock's argument list?

constraint_messages.c:136: Failure: FailureMessage -> for_no_mock_parameters_with_parameter_constraint 
	Mocked function [forgot_to_pass_parameters_mock] did not define a parameter named [x]. Did you misspell it in the expectation or forget it in the mock's argument list?

constraint_messages.c:88: Failure: FailureMessage -> for_not_equal_to_double 
	Expected [0] to [not equal double] [0] within [8] significant figures
		actual value:	0.000000
		expected value:	0.000000

constraint_messages.c:121: Failure: FailureMessage -> for_violated_never_expect 
	Mocked function [some_mock] has an expectation that it will never be called, but it was

constraint_messages.c:150: Failure: FailureMessage -> if_exception_was_expected_but_nothing_thrown 
	Expected [(void)5] to throw [std::string]

constraint_messages.c:140: Exception: FailureMessage -> increments_exception_count_when_terminating_via_SIGQUIT 
	Test terminated with signal: Terminated

constraint_messages.c:153: Exception: FailureMessage -> increments_exception_count_when_throwing 
	Test terminated unexpectedly, likely from a non-standard exception or Posix signal

Completed "FailureMessage": 0 passes, 24 failures, 3 exceptions.
Completed "constraint_messages": 0 passes, 24 failures, 3 exceptions.
