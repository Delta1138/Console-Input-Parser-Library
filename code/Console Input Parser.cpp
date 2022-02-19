#include "pch.h"
#include "framework.h"
#include "CIP.h"
#include <random>
#include <iostream>
#include <cstdarg>
#include <limits>

using namespace std;
using namespace CIP::TYPE::DIVIDERS;
#define FILE_AND_LINE (__FILE__ "(" + to_string(__LINE__) + ")") 

namespace CIP
{
	namespace
	{
		typedef unsigned char type;
		typedef const unsigned char tc; // type constant
	}


	void input_handler::setNumFlags(int x)
	{
		flags = new string[x];
		flagVars = new void**[x];
		flagDefaultVals = new void**[x];
		flagTypes = new type*[x];
		flagArgCounts = new int[x];
		flagSet = new bool*[x];
		flagSet_partOfThis = new bool[x];
		flagHasDefault = new bool[x];
		flagIsAlias = new bool[x] {}; // initialized to false...[x times]
	}
	void input_handler::setNumOrderedVars(int x)
	{
		orderedVars = new void*[x];
		orderedDefaultVals = new void*[x];
		orderedTypes = new type[x];
		orderedSet = new bool*[x];
		orderedSet_partOfThis = new bool[x];
		orderedHasDefault = new bool[x];
	}
	void input_handler::setNumTests(int x)
	{
		testFuncs = new function<bool>*[x];
		testHasFallback = new bool[x];
	}
	void input_handler::setFlagDelim(char flagDelim) { this->flagDelim = flagDelim; }
	void input_handler::setEscape(char escape) { this->escape = escape; }
	void input_handler::setHelpMessage(const char * message) { helpString = message; }
	void input_handler::setMinOrdered(int orderedMinInput) { this->orderedMinInput = orderedMinInput; orderedMinInputWasManuallySet = true; }
	void input_handler::setFlagVarDefaultMarker(std::string fVarDefaultMarker) { this->fVarDefaultMarker = fVarDefaultMarker; }
	void input_handler::setFlagFillDefaultMarker(std::string fFillDefaultMarker) { this->fFillDefaultMarker = fFillDefaultMarker; }
	void input_handler::setFlagDefaultMarkers(std::string VarMarker, std::string FillMarker) { setFlagVarDefaultMarker(VarMarker); setFlagFillDefaultMarker(FillMarker); }
	string input_handler::getPath() { return path; }
	string input_handler::getProgramName() { return programName; }
	void input_handler::preventExcessiveInput() { throwOnExcessiveInput = true; }


	input_handler::input_handler(int argc, char * argv[])
	{
		this->argc = argc;
		this->argv = new string[argc];
		for (int i = 0; i < argc; ++i)
			this->argv[i] = argv[i];


#if defined(WIN32)
		const char pathDelim = '\\';
#elif defined(__unix__)
		const char pathDelim = '\\';
#elif defined(__APPLE__)
		const char pathDelim = '\\'; // change if I learn that these are wrong
#elif defined(__MACH__)
		const char pathDelim = '\\'; // change if I learn that these are wrong
#else
		const char pathDelim = '\\'; // assume \ is the delim if OS is unknown
#endif


		// Split argv[0] into a path and program name, if applicable
		int i = this->argv[0].length();
		while (i > 0 && (argv[0][i - 1] != pathDelim))
			--i;
		if (i != 0)
		{
			path = this->argv[0].substr(0, i);
			programName = this->argv[0].substr(i);
		}
		else
			path = programName = "";
	}

	void input_handler::setFlag(string name, void * vars[], void * defaultVals[], type types[], int argc, bool * indicator)
	{
		{ // store current input
			lastVars = vars;
			lastDefaultVals = defaultVals;
			lastTypes = types;
			lastArgc = argc;
			lastIndicator = indicator;
		}

		flags[flagCount] = name;
		flagVars[flagCount] = vars;
		flagDefaultVals[flagCount] = defaultVals;
		flagTypes[flagCount] = types;
		flagArgCounts[flagCount] = argc;
		flagHasDefault[flagCount] = defaultVals != nullptr;
		if (indicator == nullptr) // effectively changes the 'indicator' passed this time, so changes lastIndicator to match
		{
			lastIndicator = flagSet[flagCount] = new bool{ false };
			flagSet_partOfThis[flagCount] = true;
		}
		else
		{
			*indicator = false;
			flagSet[flagCount] = indicator;
			flagSet_partOfThis[flagCount] = false;
		}
		++flagCount;
	}


	void input_handler::setOrdered(void * var, void * defaultVal, type t, bool * indicator)
	{
		orderedVars[orderedCount] = var;
		orderedTypes[orderedCount] = t;
		orderedDefaultVals[orderedCount] = defaultVal;
		orderedHasDefault[orderedCount] = defaultVal != nullptr;
		if (indicator == nullptr)
		{
			orderedSet[orderedCount] = new bool{ false };
			orderedSet_partOfThis[orderedCount] = true;
		}
		else
		{
			*indicator = false;
			orderedSet[orderedCount] = indicator;
			orderedSet_partOfThis[orderedCount] = false;
		}
		++orderedCount;
	}


	void CIP::input_handler::setTest(function<bool>* testFunc) { setTest(false, testFunc); }

	void CIP::input_handler::setTest(bool enableFallback, function<bool>* testFunc)
	{
		testFuncs[testCount] = testFunc;
		testHasFallback[testCount] = enableFallback;
		++testCount;
	}


	void input_handler::dynamicInput_enableRepeatedClosingTests(bool x) { IPI_repeatedFinalTestsEnabled = x; }
	void input_handler::setupDynamicInput(const char * prompts[], int tests[], const char * flagPrompt, const char * flagHelpString)
	{
		function<std::string> ** promptFuncs = new function<std::string>*[orderedCount];
		for (int i = 0; i < orderedCount; ++i)
			promptFuncs[i] = new function<std::string>(str, prompts[i]);
		setupDynamicInput(promptFuncs, tests, flagPrompt, flagHelpString);
	}
	void input_handler::setupDynamicInput(function<std::string> ** prompts, const char * flagPrompt, const char * flagHelpString)
	{
		setupDynamicInput(prompts, nullptr, flagPrompt, flagHelpString);
	}
	void input_handler::setupDynamicInput(function<std::string> ** prompts, int tests[], const char * flagPrompt, const char * flagHelpString)
	{
		in_program_input_is_enabled = true;
		IPI_prompts = prompts;
		IPI_tests = tests;
		IPI_flagsEnabled = flagPrompt != nullptr;
		IPI_flagHelpEnabled = flagPrompt != nullptr;
		IPI_flagPrompt = flagPrompt;
		IPI_flagHelp = flagHelpString;
	}




	void input_handler::setDefault(void * var, void * def, type t, int index, int flagVar)
	{ // using def = default


		if (t < func_startVal) switch (t)
		{

#define basic_case(t,ty) case t: *(ty*)var = *(ty*)def; break

			using namespace TYPE;

			basic_case(STRING, string);
			basic_case(CHAR, char);
			basic_case(UCHAR, unsigned char);
			basic_case(SHORT, short);
			basic_case(USHORT, unsigned short);
			basic_case(INT, int);
			basic_case(UINT, unsigned int);
			basic_case(LONG, long);
			basic_case(ULONG, unsigned long);
			basic_case(LONGLONG, long long);
			basic_case(ULONGLONG, unsigned long long);
			basic_case(FLOAT, float);
			basic_case(DOUBLE, double);
			basic_case(LONG_DOUBLE, long double);
			basic_case(BOOL, bool);

		case BOOLSTR: // these two must only modify the variable in their object.
			*static_cast<boolstr*>(var)->var = *(bool*)def;
			break;

		case INTSTR:
			*static_cast<intstr*>(var)->var = *(int*)def;
			break;

#undef basic_case

		}
		else if (t < ptr_startVal) switch (t)
		{

#define function_case(typeID,T)				\
		case typeID:						\
			*(T*)var << *(function<T>*)def;	\
			break

			using namespace TYPE::FUNC;

			function_case(STRING, string);
			function_case(CHAR, char);
			function_case(UCHAR, unsigned char);
			function_case(SHORT, short);
			function_case(USHORT, unsigned short);
			function_case(INT, int);
			function_case(UINT, unsigned int);
			function_case(LONG, long);
			function_case(ULONG, unsigned long);
			function_case(LONGLONG, long long);
			function_case(ULONGLONG, unsigned long long);
			function_case(FLOAT, float);
			function_case(DOUBLE, double);
			function_case(LONG_DOUBLE, long double);
			function_case(BOOL, bool);

		case BOOLSTR: // these two must only modify the variable in their object.
			*static_cast<boolstr*>(var)->var << *(function<bool>*)def;;
			break;

		case INTSTR:
			*static_cast<intstr*>(var)->var << *(function<int>*)def;;
			break;

#undef function_case

		}
		else switch (t)
		{
#define pointer_case(t,ty) case t: *(ty*)var = **(ty**)def; break

			using namespace TYPE::PTR;

			pointer_case(STRING, string);
			pointer_case(CHAR, char);
			pointer_case(UCHAR, unsigned char);
			pointer_case(SHORT, short);
			pointer_case(USHORT, unsigned short);
			pointer_case(INT, int);
			pointer_case(UINT, unsigned int);
			pointer_case(LONG, long);
			pointer_case(ULONG, unsigned long);
			pointer_case(LONGLONG, long long);
			pointer_case(ULONGLONG, unsigned long long);
			pointer_case(FLOAT, float);
			pointer_case(DOUBLE, double);
			pointer_case(LONG_DOUBLE, long double);
			pointer_case(BOOL, bool);

		case BOOLSTR: // these two must only modify the variable in their object.
			*static_cast<boolstr*>(var)->var = **(bool**)def;
			break;

		case INTSTR:
			*static_cast<intstr*>(var)->var = **(int**)def;
			break;

#undef pointer_case
		}
	}

	void input_handler::setValue(void * var, string in, type t, int index, int flagVar)
	{ // using def = default;
#ifdef _DEBUG
		if (in == "")
			throw library_programming_error(FILE_AND_LINE);
#endif

		invalid_input baseIIforTypeStrs(*this, index, flagVar, "this 'problem' string will be set by var->asssignValue() if it throws an error");


#define case(x) break; case (x)
#define setNumber(typ,inp) *(typ*)var = processNumber<typ>(inp, index, flagVar)

		switch (t % func_startVal)
		{
			using namespace TYPE;
		case(STRING):
			*(string*)var = in;

		case(CHAR):
			if (in.length() > 1)
				throw invalid_input(*this, index, flagVar, "More than one character (%i total) provided to char variable", in.length());
			else
				*(char*)var = in[0];

		case(UCHAR):
			if (in.length() > 1)
				*(unsigned char*)var = in[0];
			else
			{
				throw invalid_input(*this, index, flagVar, "More than one character (%i total) provided to unsigned char variable", in.length());
			}
			
		case(SHORT):
			setNumber(short, in);
		case(USHORT):
			setNumber(unsigned short, in);
		case(INT):
			setNumber(int, in);
		case(UINT):
			setNumber(unsigned int, in);
		case(LONG):
			setNumber(long, in);
		case(ULONG):
			setNumber(unsigned long, in);
		case(LONGLONG):
			setNumber(long long, in);
		case(ULONGLONG):
			setNumber(unsigned long long, in);
		case(FLOAT):
			setNumber(float, in);
		case(DOUBLE):
			setNumber(double, in);
		case(LONG_DOUBLE):
			setNumber(long double, in);

		case(BOOL): // false if in = f, false, or 0(000...)
			if (*(bool*)var = in != "f" && in != "false")
				for (int i = in[0] == '-' ? 1 : 0; in[i] == '0'; ++i)
					if (i + 1 == in.length())
					{
						*(bool*)var = false;
						break;
					}

		case(BOOLSTR):
			((boolstr*)var)->assignValue(in, baseIIforTypeStrs);
		case(INTSTR):
			((intstr*)var)->assignValue(in, baseIIforTypeStrs);
		}
#undef case
#undef setNumber
	}

	void input_handler::handleFlag(string flag, int * avi, int argc, string * argv) // avi = argv index. This function exists because DRY; two points need the same code which modifies that value,
	{																			   // so it's passed by reference
		for (int flg = 0; flg < flagCount; ++flg)
			if (flag == flags[flg])
			{
				*flagSet[flg] = true;

				for (int arg = 0; arg < flagArgCounts[flg]; ++arg)
				{
					if (++*avi > argc)
						throw invalid_input(*this, flg, arg + 1, "Not enough inputs provided (expected %i, got %i)", flagArgCounts, arg);

					//resolve escapes
					for (unsigned int i = 0; i < argv[*avi].length(); ++i)
						while (argv[*avi][i] == escape)
						{
							argv[*avi].erase(i++, 1); // this shifts the rest of the string left 1, which causes the escaped character to never get checked by this loop, as desired.
							if (i >= argv[*avi].length()) // this ensures no attempt is made to read past the end of the string
								break;
						}

					if (flagHasDefault[flg] && argv[*avi] == fVarDefaultMarker)
						setDefault(flagVars[flg][arg], flagDefaultVals[flg][arg], flagTypes[flg][arg], flg, arg);
					else if (flagHasDefault[flg] && argv[*avi] == fFillDefaultMarker)
						while (arg < flagArgCounts[flg])
						{
							setDefault(flagVars[flg][arg], flagDefaultVals[flg][arg], flagTypes[flg][arg], flg, arg);
							++arg;
						}
					else
						setValue(flagVars[flg][arg], argv[*avi], flagTypes[flg][arg], flg, arg);
				}
			}
	}

	input_handler::stringSplitter::stringSplitter(std::string str)
	{
		// before doing anything, eliminate any opening whitespace
		while (isspace(str[0]))
			str.erase(0);

		// first, count the number of arguments
		argc = 2; // 2 because argv[0] is always "" regardless of user input
		for (unsigned int i = 0; i < str.length(); ++i)
		{
			if (osEscape != NULL && str[i] == osEscape)
				++i;
			if (osQuote != NULL && str[i] == osQuote)
				while (i < str.length()) // note that Windows interprets [cmd som"e k"i"n"d" of i"inpu"t] as [cmd "some kind of input"]. I am deliberately imitating this.
				{
					if (osEscape != NULL && str[i] == osEscape)
						++i;
					if (str[++i] == osQuote)
						break;
				}
			if (isspace(str[i]))
				++argc;
			while (isspace(str[i]) && i < str.length())
				++i; // move past the whitespace
		}
		// then, create and populate argv, starting from 1 so that it can be trivially translated to the parent class' argv if needed
		argv = new string[argc];
		argv[0] = "";
		for (unsigned int arg = 1, i = 0, startVal = 0; arg < argc; ++i)
		{
			if (isspace(str[i]) || str[i] == NULL)
			{
				argv[arg++] = str.substr(startVal, i - startVal); // substr [startVal, i)
				while (isspace(str[i]))
					++i; // move past the whitespace
				startVal = i; // update startVal to point at the start of the next arg
			}
			while (osEscape != NULL && str[i] == osEscape)
				str.erase(i++, 1);
			if (i > str.length())
				break;
			if (osQuote != NULL && str[i] == osQuote)
			{
				str.erase(i, 1);
				while (i < str.length())
				{
					if (str[i++] == osQuote)
					{
						str.erase(i - 1, 1);
						break;
					}
					while (osEscape != NULL && str[i] == osEscape)
						str.erase(i++, 1);
				}
			}
		}
	}
	input_handler::stringSplitter::~stringSplitter()
	{
		delete[] argv;
	}

	void input_handler::runTests()
	{
		int testIndex_ignoringFallbackTests = 0;
		for (int i = 0; i < testCount; ++i) // true = success, false = fail/error
		{ // runTest() should have been its own thing with runTests() calling it. Oh well, this code never needs to be changed anyways. If it DOES get changed, find the duplicates of this comment.
			bool testResult;
			do {
				testResult << *testFuncs[i];
				if (!testResult)
					if (testHasFallback[i])
					{
						if (++i == testCount)
							throw programming_error("The final test claims to have a fallback test, but one is not present");
					}
					else
						throw test_failed(testIndex_ignoringFallbackTests);
			} while (!testResult);
			while (testHasFallback[i]) // goto end of test sequence
				++i;
			++testIndex_ignoringFallbackTests;
		}
	}

	void input_handler::interpretInput(ostream& output, istream& input)
	{
		// step 1: check if input is requesting help
		if (argc > 1 && (argv[1] == "/?" || argv[1] == (flagDelim + "help")))
		{
			output << helpString;
			throw user_requested_help();
		}

		if (!orderedMinInputWasManuallySet)
			orderedMinInput = orderedCount;

		if (argc > 1 || (orderedMinInput == 0 && !in_program_input_is_enabled)) // if run from cmdline.
		{																	   // if 0 input is permitted and input, run ipi if enabled, else run this.
			int orderedIndex = 0; // holds the index of ordered var which is next to be input, and the number of ordered vars which have been input
			for (int i = 1; i < argc; ++i) // read input
			{
				if (argv[i][0] == flagDelim)
				{
					string flag = argv[i].substr(1);
					handleFlag(flag, &i, argc, argv);
				}
				else
				{
					//  it is possible for valid input to need to start with the flag delim, so an escape character is accepted to avoid that. Time to resolve escapes.
					for (unsigned int j = 0; j < argv[i].length(); ++j)
						while (argv[i][j] == escape)
						{
							argv[i].erase(j++, 1); // this shifts the rest of the string left 1, which causes the escaped character to never get checked by this loop, as desired.
							if (j >= argv[i].length()) // this ensures no attempt is made to read past the end of the string
								break;
						}
					if (orderedIndex < orderedCount)
					{
						*orderedSet[orderedIndex] = true;
						setValue(orderedVars[orderedIndex], argv[i], orderedTypes[orderedIndex], orderedIndex);
						++orderedIndex;
					}
					else if (throwOnExcessiveInput)
						throw excessive_input(argc - 1, orderedCount);
				}
			}
			// check that enough orderedVars were input
			if (orderedIndex < orderedMinInput)
				throw not_enough_inputs(orderedIndex, orderedMinInput);

			// set defaults

			for (int i = 0; i < flagCount; ++i)
				if (!flagIsAlias[i] && !*flagSet[i] && flagHasDefault[i])
					for (int j = 0; j < flagArgCounts[i]; ++j)
						setDefault(flagVars[i][j], flagDefaultVals[i][j], flagTypes[i][j], i, j);

			for (int i = orderedIndex; i < orderedCount; ++i)
				if (orderedHasDefault[i])
					setDefault(orderedVars[i], orderedDefaultVals[i], orderedTypes[i], orderedIndex);

			runTests();
		}
		else if (IPI_imitateCmdLine)
		{
			string tempString = "";
			while (tempString == "")
			{
				output << *IPI_prompts[0];
				getline(input, tempString);
				if (tempString == "")
					output << "ERROR: no input provided.\n\n";
			}
			stringSplitter args(tempString);
			argc = args.argc;
			argv = args.argv;
			try {
				interpretInput();
			}
			catch (...)
			{
				argv = nullptr;
				throw;
			}
			argv = nullptr; // since args is about to fall out of scope, its destructor will be called and argv destroyed. As such, we need to avoid destroying argv a second time
		}
		else if (in_program_input_is_enabled)
		{
			string tempString = " ";
			for (int i = 0; i < orderedCount; ++i)
			{
				output << *IPI_prompts[i];

				if(tempString != "") 
					getline(input, tempString);

				// check if input is empty and assign value (input or default)

				if (tempString != "")
				{
					*orderedSet[i] = true;
					setValue(orderedVars[i], tempString, orderedTypes[i], i);
				}
				else if (orderedHasDefault[i])
					setDefault(orderedVars[i], orderedDefaultVals[i], orderedTypes[i], i);
				else if (i < orderedMinInput)
					throw invalid_input(*this, i, "IPI - Ordered Varaible %i was not set", i);

				// run test (if applicable)

				if (IPI_tests != nullptr && IPI_tests[i] >= 0 && IPI_tests[i] < testCount)
				{

					int test = 0;
					for (int testSequencesRemaining = IPI_tests[i]; testSequencesRemaining > 0; ++test)
						if (testHasFallback[test] == false)
							--testSequencesRemaining;

					bool testResult;
					do { // runTest() should have been its own thing with runTests() calling it. Oh well, this code never needs to be changed anyways
						testResult << *testFuncs[test];
						if (!testResult)
							if (testHasFallback[test])
							{
								if (++test == testCount)
									throw programming_error("The final test claims to have a fallback test, but one is not present");
							}
							else
								throw test_failed(IPI_tests[i]);
					} while (!testResult);
				}
			}
			if (IPI_flagsEnabled)
			{
			IPI_FLAGS:
				output << IPI_flagPrompt;
				string tempString;
				getline(input, tempString);
				if (IPI_flagHelpEnabled && (tempString == "?" || tempString == "help"))
				{
					output << IPI_flagHelp;
					goto IPI_FLAGS;
				}
				if (tempString != "")
				{
					stringSplitter flin(tempString);
					for (int i = 0; i < static_cast<signed int>(flin.argc); ++i)
						handleFlag(flin.argv[i], &i, flin.argc, flin.argv);
				}
			}

			// set flag defaults
			for (int i = 0; i < flagCount; ++i)
				if (!flagIsAlias[i] && !*flagSet[i] && flagHasDefault[i])
					for (int j = 0; j < flagArgCounts[i]; ++j)
						setDefault(flagVars[i][j], flagDefaultVals[i][j], flagTypes[i][j], i, j);


			// Run all of the tests (some for the second time) to make absolutely sure everything's in order, unless this final check has been disabled
			if ((IPI_tests == nullptr) || IPI_repeatedFinalTestsEnabled)
				runTests();
			else
			{
				int numTestSets = testCount;
				for (int i = 0; i < testCount; ++i)
					if (testHasFallback[i])
						--numTestSets;
				int * testIndexes = new int[numTestSets] {0}; // the first of this array should be 0
				for (int i = 0, tii = 1; i < testCount; ++i)
					if (!testHasFallback[i])
						testIndexes[tii++] = i + 1;
				try {
					for (int i = 0; i < numTestSets; ++i)
					{
						for (int j = 0; j < orderedCount; ++j)
							if (i == IPI_tests[j])
								goto skip_this_test_which_has_already_run;

						{ // runTest() should have been its own thing with runTests() calling it. Oh well, this code never needs to be changed anyways.
							bool testResult;
							int j = testIndexes[i];
							do {
								testResult << *testFuncs[j];
								if (!testResult)
									if (testHasFallback[j])
									{
										if (++j == testCount)
											throw programming_error("The final test claims to have a fallback test, but one is not present");
									}
									else
										throw test_failed(i);
							} while (!testResult);
						}

					skip_this_test_which_has_already_run:;
					}
				}
				catch (...) {
					delete[] testIndexes;
					throw;
				}
				delete[] testIndexes;
			}
		}
		else
			throw not_enough_inputs(0, orderedMinInput);
	}


#define del_help_case(ary,var,t,ty) case t: delete##ary (ty*)var; break

#define del_help_cases(ary,var,t,ty)				\
	del_help_case(ary, var, t, ty);					\
	del_help_case(ary, var, FUNC::t, function<ty>);	\
	del_help_case(ary, var, PTR::t, ty*);

#define del_help_switch(var,ary)										\
			del_help_cases(ary, var, STRING, string);					\
			del_help_cases(ary, var, CHAR, char);						\
			del_help_cases(ary, var, UCHAR, unsigned char);				\
			del_help_cases(ary, var, SHORT, short);						\
			del_help_cases(ary, var, USHORT, unsigned short);			\
			del_help_cases(ary, var, INT, int);							\
			del_help_cases(ary, var, UINT, unsigned int);				\
			del_help_cases(ary, var, LONG, long);						\
			del_help_cases(ary, var, ULONG, unsigned long);				\
			del_help_cases(ary, var, LONGLONG, long long);				\
			del_help_cases(ary, var, ULONGLONG, unsigned long long);	\
			del_help_cases(ary, var, FLOAT, float);						\
			del_help_cases(ary, var, DOUBLE, double);					\
			del_help_cases(ary, var, LONG_DOUBLE, long double);			\
			del_help_cases(ary, var, BOOL, bool);						\
			del_help_cases(ary, var, BOOLSTR, boolstr);					\
			del_help_cases(ary, var, INTSTR, intstr);

	// the difference between del and delete is that, when used on void pointers, del calls destructors of types where applicable but delete doesn't
#define del(var,t,ary) do{\
		switch (t)\
		{\
			using namespace TYPE;\
			del_help_switch(var,ary);\
		} } while (false)

	input_handler::~input_handler()
	{
		delete[] argv;

		for (int i = 0; i < orderedCount; ++i)
		{
			delete IPI_prompts[i];
			del(orderedDefaultVals[i], orderedTypes[i],);
			if (orderedSet_partOfThis[i])
				delete orderedSet[i];
		}
		delete[] orderedVars;
		delete[] orderedHasDefault;
		delete[] orderedTypes;
		delete[] orderedSet;
		delete[] orderedSet_partOfThis;																																				   
		delete[] IPI_tests;
		delete[] IPI_prompts;
		delete[] orderedDefaultVals;

		for (int i = 0; i < flagCount; ++i)
			if (!flagIsAlias[i])
			{
				if (flagDefaultVals[i])
				{
					bool all_elements_are_same_type_and_dont_have_func_defaults_thus_were_all_created_as_one_array = true;
					type firstType = func_startVal;
					if (flagTypes[i])
						firstType = flagTypes[i][0];

					if (firstType >= func_startVal && firstType < ptr_startVal)
						all_elements_are_same_type_and_dont_have_func_defaults_thus_were_all_created_as_one_array = false;
					else for(int j = 1; j < flagArgCounts[i]; ++j)
						if (flagTypes[i][j] != firstType)
						{
							all_elements_are_same_type_and_dont_have_func_defaults_thus_were_all_created_as_one_array = false;
							break;
						}

					if (all_elements_are_same_type_and_dont_have_func_defaults_thus_were_all_created_as_one_array)
						del(flagDefaultVals[i][0], firstType, []);
					else
						for (int j = 0; j < flagArgCounts[i]; ++j)
							del(flagDefaultVals[i][j], flagTypes[i][j], );
				}
				delete[] flagDefaultVals[i];
				delete[] flagTypes[i];
				delete[] flagVars[i];
				if (flagSet_partOfThis[i])
					delete flagSet[i];
			}
		delete[] flags;
		delete[] flagArgCounts;
		delete[] flagHasDefault;
		delete[] flagIsAlias;
		delete[] flagTypes;
		delete[] flagDefaultVals;
		delete[] flagVars;
		delete[] flagSet;
		delete[] flagSet_partOfThis;

		for (int i = 0; i < testCount; ++i)
			delete testFuncs[i];
		delete[] testFuncs;
		delete[] testHasFallback;
	}

#undef del
#undef del_help_case
#undef del_help_cases
#undef del_help_switch		

	// main setFlag func:
   // void input_handler::setFlag(string name, void * vars[], void * defaultVals[], type types[], int argc, bool * indicator)

	void input_handler::setFlag(string name, void * vars[], type types[], int argc, bool * indicator)
	{
		setFlag(name, vars, nullptr, types, argc, indicator);
	}

	void input_handler::setFlag(string name, bool * indicator)
	{
		setFlag(name, nullptr, nullptr, nullptr, 0, indicator);
	}

	void input_handler::setFlag(string name)
	{
		flagIsAlias[flagCount] = true;
		if (lastArgc < 0) // lastArgc is set to -1 on object's creation, but otherwise can never be less than 1 unless the programmer does stupid things with causing and ignoring exceptions.
			programming_error("The first flag cannot be an alias of the preceeding flag");
		setFlag(name, lastVars, lastDefaultVals, lastTypes, lastArgc, lastIndicator);
	}


	void input_handler::setupDynamicInput(const char * prompts[], const char * flagPrompt, const char * flagHelpString) { setupDynamicInput(prompts, nullptr, flagPrompt, flagHelpString); }
	void input_handler::setupDynamicInput(const char * prompt)
	{
		IPI_imitateCmdLine = true;
		setupDynamicInput(&prompt, nullptr, "", "");
	}


	void input_handler::debug_listOutput()
	{
		debug_listOutput(std::cout, 0);
	}


	// specialisations of getTypeConstant:

#define helper(n,t,ty) template<> tc input_handler::getTypeConstant<ty>() { return n::t; }
#define Define(t,ty) 										\
			helper(TYPE,t,ty) 								\
			helper(TYPE::FUNC,t,ty(*)(unsigned int,void**)) \
			helper(TYPE::FUNC,t,function<ty>)				\
			helper(TYPE::FUNC,t,function<ty>*)				\
			helper(TYPE::PTR,t,ty*)
	
	Define(STRING, std::string)
	Define(CHAR, char)
	Define(UCHAR, unsigned char)
	Define(SHORT, short)
	Define(USHORT, unsigned short)
	Define(INT, int)
	Define(UINT, unsigned int)
	Define(LONG, long)
	Define(ULONG, unsigned long)
	Define(LONGLONG, long long)
	Define(ULONGLONG, unsigned long long)
	Define(FLOAT, float)
	Define(DOUBLE, double)
	Define(LONG_DOUBLE, long double)
	Define(BOOL, bool)
	Define(BOOLSTR, boolstr)
	Define(INTSTR, intstr)

#undef Define
#undef helper
}

/*
	string defaultNewFilename(string o, bool c) // o = original filename | c = creating
	{
		// first, find o's file extension (if applicable)
		int i = o.length() - 1;
		while (i > 0 && o[i] != '.' && o[i] != '\\' && o[i] != '/')
			--i;
		string marker = (c ? "_patch.dbp" : ("_patched" + (o[i] == '.' ? o.substr(i) : "")));
		if (o[i] == '.')
			return (o.substr(0, i) + marker);
		return (o + marker);
	}
*/