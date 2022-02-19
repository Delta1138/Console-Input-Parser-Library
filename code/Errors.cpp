#include <iostream>
#include "pch.h"
#include "framework.h"
#include "CIP.h"

using namespace std;
#define FILE_AND_LINE (__FILE__ "(" + to_string(__LINE__) + ")")

namespace CIP
{
	test_failed::test_failed(int index) { this->index = index; problem = "Test " + to_string(index + 1) + " failed"; }
	const char * test_failed::what() { return problem.c_str(); }
	const char * invalid_input::what() { return problem.c_str(); }
	const char * not_enough_inputs::what() { return problem.c_str(); }
	const char * excessive_input::what() { return problem.c_str(); }
	library_programming_error::library_programming_error(string fileAndLine) { this->fileAndLine = fileAndLine;  }
	const char * library_programming_error::what() { return fileAndLine.c_str(); }

	not_enough_inputs::not_enough_inputs(int inputCount, int minIn) : numIn{ inputCount }
	{
		problem = "A minimum of " + to_string(minIn) + " non-flag inputs must be provided (recieved " + to_string(inputCount) + ")";
	}

	excessive_input::excessive_input(int inputCount, int maxIn) : numIn{ inputCount }
	{
		problem = "A maximum of " + to_string(maxIn) + " non-flag inputs may be provided (recieved " + to_string(inputCount) + ")";
	}
}