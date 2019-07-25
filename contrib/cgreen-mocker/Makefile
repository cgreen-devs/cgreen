all:
	@echo "With the command:"
	@echo "-----------------"
	./cgreen-mocker.py double.h > double.mock
	@echo "-----------------"
	@echo "'cgreen-mocker' reads a C header file and transforms any"
	@echo "function declarations into corresponding Cgreen mocks."
	@echo "Here's the input:"
	@echo "-----------------"
	@cat double.h
	@echo "-----------------"
	@echo "And here's the resulting mock declaration:"
	@echo "------------------------------------------"
	@cat double.mock
	@echo "------------------------------------------"
	@echo "With the command:"
	@echo "-----------------"
	./cgreen-mocker.py complex_types.h > complex_types.mock
	@echo "-----------------"
	@echo "'cgreen-mocker' reads a C header file and transforms any"
	@echo "function declarations into corresponding Cgreen mocks."
	@echo "Here's the input:"
	@echo "-----------------"
	@cat complex_types.h
	@echo "-----------------"
	@echo "And here's the resulting mock declaration:"
	@echo "------------------------------------------"
	@cat complex_types.mock
	@echo "------------------------------------------"
