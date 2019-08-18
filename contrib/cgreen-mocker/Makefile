all:
	@echo "Testing..."
	@echo -n "double... "
	@./cgreen-mocker.py double.h > double.mock
	@diff double.mock double.mock.expected
	@if [ "$$?" -eq 0 ] ; then echo "Ok" ; fi
	@echo -n "complex_types... "
	@./cgreen-mocker.py complex_types.h > complex_types.mock
	@diff complex_types.mock complex_types.mock.expected
	@if [ "$$?" -eq 0 ] ; then echo "Ok" ; fi
	@echo -n "simple_types... "
	@./cgreen-mocker.py simple_types.h > simple_types.mock
	@diff simple_types.mock simple_types.mock.expected
	@if [ "$$?" -eq 0 ] ; then echo "Ok" ; fi
	@echo -n "multiple_types... "
	@./cgreen-mocker.py multiple_types.h > multiple_types.mock
	@diff multiple_types.mock multiple_types.mock.expected
	@if [ "$$?" -eq 0 ] ; then echo "Ok" ; fi
