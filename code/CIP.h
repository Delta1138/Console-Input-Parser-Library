#pragma once
// marks the start of CIP flag declarations. 
// Place 1 declaration per line following this (with no blank lines)
// Place CIP_FLAGS_END on the line after the last declaration
#define CIP_FLAGS_START(input_handler) const int CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_START = __LINE__; goto CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_E; CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_S:
#define CIP_FLAGS_END(input_handler) goto CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_F; CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_E: (input_handler).setNumFlags(__LINE__ - CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_START - 1); goto CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_S; CIP_##input_handler##_GBHJSGJHTRTGSSVJCHBNA_TAG_F: // marks the end of CIP flag declarations
// marks the start of CIP ordered variable declarations. 
// Place 1 declaration per line following this (with no blank lines)
// Place CIP_ORDERED_END on the line after the last declaration
#define CIP_ORDERED_START(input_handler) const int CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_START = __LINE__; goto CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_E; CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_S:
#define CIP_ORDERED_END(input_handler) goto CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_F; CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_E: (input_handler).setNumOrderedVars(__LINE__ - CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_START - 1); goto CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_S; CIP_##input_handler##_JHFDBJH0WIQOWPCKVCERC_TAG_F: // marks the end of CIP ordered var declarations
// marks the start of CIP test declarations. 
// Place 1 declaration per line following this (with no blank lines)
// Place CIP_TESTS_END on the line after the last declaration
#define CIP_TESTS_START(input_handler) const int CIP_##input_handler##_987fyu54387fby8hn5bsa_START = __LINE__; goto CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_E; CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_S:
#define CIP_TESTS_END(input_handler) goto CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_F; CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_E: (input_handler).setNumTests(__LINE__ - CIP_##input_handler##_987fyu54387fby8hn5bsa_START - 1); goto CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_S; CIP_##input_handler##_987fyu54387fby8hn5bsa_TAG_F: // marks the end of CIP test declarations

#define CIP_strFs(x) new function<std::string>*[x]
#define CIP_strs(x) new const char*[x]
#define CIP_LAMBDA(type, name) type (*name)(unsigned int, void**) = [](unsigned int argc, void** argv) -> type

// TODO: consider whether adding the macro #define CIP_x(ty, vp[x]) *(ty*)(void**)vp[x] would be a good idea because of the way in which functions passed to this lib must work
// TODO: consider if something like smart pointers could be used to prevent memory leaks (if I had functions which returned them for say void arrays of new ints rather than using macros for that)
// TODO: split this header into four, where the other three contain the template definitions and this one only has declarations. <stdarg> and <iostream> can be removed from this header at that point.
//       there should be 1 .h for each .cpp. Alternatively, just shove definitions below the namespace.
// TODO: where cout is explicitly referenced, change it to a reference to a generic ostream that can be manually set.

#include <string>
#include <stdarg.h>
#include <iostream>

namespace CIP
{
	const signed char no_test = -1;


	// BOOLSTR - struct which contains a pointer-to-bool and true/false match rules.
	// INTSTR - similar to boolstr, except can hold more patterns
	// they both must have an operator<<(ostream, self) which prints the value of what they are pointing at.

	class invalid_input;

	struct boolstr
	{
		bool* var;
		void assignValue(std::string input, invalid_input * exception = nullptr);
		void assignValue(std::string input, invalid_input& exception);
		template<class... T> inline boolstr(bool & var, bool cstringsEqual, T... cstrings);
		~boolstr();
		template<class... T> void setMarkersFor(bool cstringsEqual, T... cstrings);


	private:
		const char** trueStrings = nullptr;
		const char** falseStrings = nullptr;
		const char*** strings[2]{ &falseStrings, &trueStrings };
		int numTrueStrings = 0;
		int numFalseStrings = 0;
		int * numStrings[2]{ &numFalseStrings, &numTrueStrings };
	};
	std::ostream& operator<<(std::ostream &x, boolstr &y);
	
	struct intstr
	{
		int* var;
		template<class... T> intstr(int & var, int cstringsEqual, T... cstrings);
		~intstr();
		template<class... T> void setMarkersFor(bool cstringsEqual, T... cstrings);
		void assignValue(std::string input, invalid_input * exception = nullptr);
		void assignValue(std::string input, invalid_input& exception);

	private:
		int exists(int x); // returns -1 if no match, index of match if there is one
		int lengthOfLongestCorrespondsToValue; // length of the longest correspondsTo value in data, in characters required to represent it.
		int lengthOfValue(int x); // returns the length of x, in characters required to represent it.
		struct Data
		{
			const char ** string;
			unsigned int numStrings;
			int correspondsTo;
			~Data();
		};
		Data * data;
		unsigned int numData = 0;
	};
	std::ostream& operator<<(std::ostream &x, intstr &y);

	namespace TYPE
	{
		typedef unsigned char type;
		typedef const type tc;
	}
	using TYPE::type;
	using TYPE::tc;

	

	namespace TYPE
	{
		namespace DIVIDERS
		{
			tc func_startVal = 17;
			tc ptr_startVal = func_startVal * 2;
			tc integralTypeStart = 3;
			tc floatingTypeStart = 11;
			tc floatingTypeEnd = 13;
			static_assert(func_startVal * 3 - 1 < 256, "Typedefs \"type\" and \"tc\" must be updated to hold values larger than 255");
		}

#define TYPE_HELPER(startVal)													\
tc STRING		  = startVal + 0;												\
tc CHAR			  = startVal + 1;												\
tc UCHAR		  = startVal + 2;												\
tc SHORT		  = startVal + 3;												\
tc USHORT		  = startVal + 4;												\
tc INT			  = startVal + 5;												\
tc UINT			  = startVal + 6;												\
tc LONG			  = startVal + 7;												\
tc ULONG		  = startVal + 8;												\
tc LONGLONG		  = startVal + 9;												\
tc ULONGLONG	  = startVal + 10;												\
tc FLOAT		  = startVal + 11;												\
tc DOUBLE		  = startVal + 12;												\
tc LONG_DOUBLE	  = startVal + 13;												\
tc BOOL			  = startVal + 14;												\
tc BOOLSTR		  = startVal + 15;												\
tc INTSTR		  = startVal + 16;												\
 /* when a type is added to this macro, func_startVal must be updated. */	\
/* if a type is added before LONG_DOUBLE, update the TypeStart constants.  */	\
tc STR	= STRING;																\
tc C	= CHAR;																	\
tc UC	= UCHAR;																\
tc S	= SHORT;																\
tc US	= USHORT;																\
tc I	= INT;																	\
tc UI	= UINT;																	\
tc L	= LONG;																	\
tc UL	= ULONG;																\
tc F    = FLOAT;																\
tc D	= DOUBLE;																\
tc LD	= LONG_DOUBLE;															\
tc LL	= LONGLONG;																\
tc ULL	= ULONGLONG;															\
tc BSTR = BOOLSTR;																\
tc ISTR = INTSTR																

		TYPE_HELPER(0);
		namespace FUNC { TYPE_HELPER(DIVIDERS::func_startVal); }
		namespace PTR  { TYPE_HELPER(DIVIDERS::ptr_startVal);  }

#undef TYPE_HELPER
	}

	/*namespace errm
	{
		
	}/**/

	class test_failed : public std::exception
	{
		std::string problem;
	public:
		const char * what();
		int index;
		test_failed(int index);
	};

	class user_requested_help : public std::exception {};
	class not_enough_inputs : public std::exception 
	{
		std::string problem;
	public:
		const char * what();
		int numIn;
		not_enough_inputs(int inputCount, int minIn);
	};

	class excessive_input : public std::exception 
	{
		std::string problem;
	public:
		const char * what();
		int numIn;
		excessive_input(int inputCount, int maxIn);
	};

	class input_handler;

	class invalid_input : public std::exception
	{
		std::string problem;
		input_handler * IH;

		template <class... T> invalid_input(input_handler& IH, bool type, int index, int subIndex, std::string problem, T... problemArgs); // defined at the bottom of this header.

		friend void boolstr::assignValue(std::string, invalid_input*);
		friend void intstr::assignValue(std::string, invalid_input*);

	public:
		const char * what();
		bool type;
		static const bool ordered = true;
		static const bool flag = false;
		static const int notAflag = -1; // arguably should not be public and should only be accessed by friends
		int index;
		std::string name;

		template <class... T>
		invalid_input(input_handler& IH, int index, std::string problem, T... problemArgs);

		template <class... T>
		invalid_input(input_handler& IH, int index, int subIndex, std::string problem, T... problemArgs);
	};


	class programming_error : public std::exception 
	{
		const char * problem;
	public:
		const char * what() { return problem; }
		programming_error(const char * problem) { this->problem = problem; }
	};

	class library_programming_error : public std::exception // for cases where something happens that should not be possible given this library's implementation
	{
		std::string fileAndLine;
	public:
		const char * what();
		library_programming_error(std::string fileAndLine);
	};





	template <class T>
	class function
	{
		unsigned int * deleteArgOnDestruction = nullptr; // 0 = no, 1 = delete, 2 = delete[], 3+ = delete each element of the array, then delete[], where the array has # - 2 elements.
		type * typesOfArgsToDelete = nullptr;
		unsigned int argc = 0;
		void ** argv = nullptr;
		T(*func)(unsigned int, void**) = nullptr;
	public:
		template <class... Ts>
		function(T(*func)(unsigned int, void**), Ts...);
		function(T(*func)(unsigned int, void**), unsigned int, void**, unsigned int *, type *);
		// function() {} // this used to have a purpose, but that purpose has been lost with the loss of this class' friends.

		function(const function&) = delete;
		function& operator=(const function&) = delete;

		~function();
		T value();
	};

	template<class T>
	inline T& operator>>(function<T>& y, T& x) { x = y.value(); return y; }
	template<class T>
	inline T& operator<<(T& x, function<T>& y) { x = y.value(); return x; }
	template<class T>
	inline std::ostream& operator<<(std::ostream& x, function<T>& y) { return x << y.value(); }

	template<class T>
	inline T function<T>::value() { return func(argc, argv); }

	template<class T>
	template<class... Ts>
	inline function<T>::function(T(*func)(unsigned int, void**), Ts... args) : func{ func }, argc{ sizeof...(args) }
	{ // if this constructor is ever modified, modify NOT() to match, and refactor everything so that I don't need to double edit again in the future.
		unsigned int tempArgc = sizeof...(args);

		va_list l;
		va_list l2;
		va_start(l, func);
		va_copy(l2, l);

		for (unsigned int i = 0; i < tempArgc; ++i)
			if (va_arg(l, void*) == nullptr)
			{
				for (void * temp = nullptr; temp == nullptr && i < tempArgc; temp = va_arg(l, void*), ++i)
					--argc;
				--argc;
			}

		if (argc > 0)
		{
			argv = new void*[argc];
			deleteArgOnDestruction = new unsigned int[argc] {0};
			typesOfArgsToDelete = new type[argc + 1] {0};
			for (unsigned int i = 0, arg = 0; i < tempArgc; ++i)
			{
				void * temp = va_arg(l2, void*);
				if (temp)
				{
					argv[arg] = temp;
					++arg;
				}
				else 
				{
					while (!temp && i < tempArgc)
					{
						++deleteArgOnDestruction[arg];
						temp = va_arg(l2, void*);
						++i;
					}
					if (temp)
					{
						typesOfArgsToDelete[arg] = *(type*)temp;
						delete temp;
					}
				}
			}
		}

		va_end(l);
		va_end(l2);
	}
	template<class T>
	inline function<T>::function(T(*func)(unsigned int, void **), unsigned int argc, void ** argv, unsigned int * deleteArgOnDestruction, type * typesOfArgsToDelete) : 
		func{ func }, argc{ argc }, argv{ argv }, deleteArgOnDestruction{ deleteArgOnDestruction }, typesOfArgsToDelete{ typesOfArgsToDelete } {}


#define del_help_case(b,p,var,s,t,ty) case t: delete##b ((ty*p)var)s; break

#define del_help_cases(b,p,var,s,t,ty)					\
	del_help_case(b, p, var, s,       t, ty);			\
	del_help_case(b, p, var, s, FUNC::t, function<ty>);	\
	del_help_case(b, p, var, s,  PTR::t, ty*);

#define del_help_switch(b,p,var,s)										 \
			del_help_cases(b, p, var, s, STRING, std::string);			 \
			del_help_cases(b, p, var, s, CHAR, char);					 \
			del_help_cases(b, p, var, s, UCHAR, unsigned char);			 \
			del_help_cases(b, p, var, s, SHORT, short);					 \
			del_help_cases(b, p, var, s, USHORT, unsigned short);		 \
			del_help_cases(b, p, var, s, INT, int);						 \
			del_help_cases(b, p, var, s, UINT, unsigned int);			 \
			del_help_cases(b, p, var, s, LONG, long);					 \
			del_help_cases(b, p, var, s, ULONG, unsigned long);			 \
			del_help_cases(b, p, var, s, LONGLONG, long long);			 \
			del_help_cases(b, p, var, s, ULONGLONG, unsigned long long); \
			del_help_cases(b, p, var, s, FLOAT, float);					 \
			del_help_cases(b, p, var, s, DOUBLE, double);				 \
			del_help_cases(b, p, var, s, LONG_DOUBLE, long double);		 \
			del_help_cases(b, p, var, s, BOOL, bool);					 \
			del_help_cases(b, p, var, s, BOOLSTR, boolstr);				 \
			del_help_cases(b, p, var, s, INTSTR, intstr);

	// the difference between del and delete is that, when used on void pointers, del calls destructors of types where applicable but delete doesn't
#define del(brack,ptr,var,sub,t) do{\
		switch (t)\
		{\
			using namespace TYPE;\
			del_help_switch(brack,ptr,var,sub);\
		} } while (false)

	template<class T>
	inline function<T>::~function()
	{
		for (unsigned int i = 0; i < argc; ++i)
			if (deleteArgOnDestruction[i])
			{
				if (deleteArgOnDestruction[i] > 2)
				{
					while (deleteArgOnDestruction[i] > 2)
					{
						del(,*, argv[i], [deleteArgOnDestruction[i] - 3], typesOfArgsToDelete[i]);
						--deleteArgOnDestruction[i];
					}
					del([], *, argv[i],, typesOfArgsToDelete[i]);
				}
				else if (deleteArgOnDestruction[i] == 2)
					del([],, argv[i],, typesOfArgsToDelete[i]);
				else
					del(,, argv[i],, typesOfArgsToDelete[i]);
			}

		delete[] typesOfArgsToDelete;
		delete[] deleteArgOnDestruction;
		delete[] argv;
	}
#undef del
#undef del_help_case
#undef del_help_cases
#undef del_help_switch	





	class input_handler
	{
		struct stringSplitter // TODO: constructor throws exception.
		{
			unsigned int argc;
			std::string * argv;
			stringSplitter(std::string str);
			~stringSplitter();
		private:
#if defined(WIN32) // TODO: figure out what the values for other OSs are
			static const char osEscape = '^';
			static const char osQuote = '"';
#elif defined(__unix__)
			static const char osEscape = NULL;
			static const char osQuote = NULL;
#elif defined(__APPLE__)
			static const char osEscape = NULL;
			static const char osQuote = NULL;
#elif defined(__MACH__)
			static const char osEscape = NULL;
			static const char osQuote = NULL;
#else
			static const char osEscape = NULL;
			static const char osQuote = NULL;
#endif
#ifndef CIP_SUPPRESS_OS_DELIMS_WARNING
			static_assert(osEscape != NULL || osQuote != NULL, "(" __FILE__ ") ERROR/WARNING: osEscape and osQuote are not defined for this build target "\
			"(which means that if dynamic input imitating cmd line is enabled, the escape character and quote character will not be processed properly). "\
			"#define CIP_SUPPRESS_OS_DELIMS_WARNING to suppress this error");
#endif // !CIP_SUPPRESS_OS_DELIMS_WARNING
			static_assert(osEscape != osQuote && osEscape != NULL, "ERROR: (input_handler::stringSplitter) osEscape and osQuote are equal and not NULL");
		};

		input_handler(const input_handler&) = delete;			   // copying an input_handler is a concept which makes no sense. It would essentially just be (IH a, IH b = &a)
		input_handler& operator=(const input_handler&) = delete;  // except that it would behave slightly differently than that in ways which are always harmful. As such, doing so is not allowed.

		int argc; // holds the number of inputs provided
		std::string * argv = nullptr; // holds the inputs as strings
		std::string path; // holds argv[0], interpreted as a path with any trailing program name removed
		std::string programName; // holds argv[0], interpreted as a program name with any preceeding path removed
		// default flag error handling | exit on call to help |  
		char errorFlags = 0b00000000;
		bool throwOnExcessiveInput = false;

		char flagDelim = '/';
		char escape = '\\';
		std::string fVarDefaultMarker = "~";
		std::string fFillDefaultMarker = "~~";
		const char * helpString = "The creator of this program neglected to create a help string, so you're on your own! Good luck!";


		// array of strings
		std::string * flags = nullptr;

		// array of ptrs to [arrays of ptrs to vars] / [nullptrs]
		// these are the variables which hold the arguments of flags
		void *** flagVars = nullptr;

		// array of ptrs to {arrays of [ptrs to ptrs to vars] / [ptrs to array with ary[0] = function, ary[1] = argc, ary[...] = args]} / {nullptrs}

		// array of ptrs to [arrays of ptrs to vars (which can be function arrays)] / [nullptrs]
		// these are the variables which hold the functions used to create default input for flags
		void *** flagDefaultVals = nullptr;

		// array of ptrs to [arrays of types] / [nullpters]
		// these are the types which the flagVars are pointing to
		type ** flagTypes = nullptr;

		// array of ints
		// these are the amounts of arguments which each flag has
		int * flagArgCounts = nullptr;

		// array of ptrs to bools
		// these hold whether a flag was set
		bool ** flagSet = nullptr;

		// array of bools
		// these hold whether a given flagSet bool is owned by this class or not
		bool * flagSet_partOfThis = nullptr;

		// array of bools
		// these hold whether a flag has a default value or not
		bool * flagHasDefault = nullptr;

		// array of bools
		// these hold whether a flag is an alias for the last flag
		bool * flagIsAlias = nullptr;

		int flagCount = 0; // holds the flag which is currently being written to before interpreting, and the number of flags set during interpreting

		// all of these record the last input to the lead setFlag(), which is used by the copying setFlag() to send a copy of the last arguments to the lead.
		void ** lastVars = nullptr; //
		void ** lastDefaultVals = nullptr;
		type * lastTypes = nullptr;
		int lastArgc = -1;
		bool * lastIndicator = nullptr;


		// array of pointers to vars
		// these are the variables which unflagged input will be put into
		void ** orderedVars = nullptr;

		// array of pointers to vars (which can be functions) / nullptrs.
		// these are the variables which hold the functions used to create default input for unflagged input
		void ** orderedDefaultVals = nullptr;

		// array of types
		// these are the types which the orderedVars are pointing to
		type * orderedTypes = nullptr;

		// array of ptrs to bools
		// these bools hold whether an ordered var was set
		bool ** orderedSet = nullptr;

		// array of bools
		// these hold whether a given orderedSet bool is owned by this class or not
		bool * orderedSet_partOfThis = nullptr;

		// array of bools
		// these hold whether an ordered var has a default value or not
		bool * orderedHasDefault = nullptr;

		int orderedCount = 0; // holds the ordered variable which is currently being written to before interpreting, and the number of ordered vars set during interpreting

		int orderedMinInput; // the number of ordered vars which the user must set
		bool orderedMinInputWasManuallySet = false;


		bool in_program_input_is_enabled = false; // if true, the program will prompt the user to input when it is run without input

		bool IPI_imitateCmdLine = false; // if true, display the first prompt, take one input, and interpret it as a cmd line input.

		function<std::string> ** IPI_prompts = nullptr; // holds the prompts which will be displayed to the user for each ordered input, as function arrays (is an array of function arrays)

		// nullptr or array of ints, where the ints correspond to the test which should be run after each input, and ints which don't (such as -1) are safely ignored
		// note that only 1 test can run per IPI input. This is by design, for simplicity. It can be worked around using the and() or() not() xor() functions I will make. (// TODO: )
		const int * IPI_tests = nullptr;

		bool IPI_flagsEnabled = false; // holds whether the user can input flags during in-program-input

		bool IPI_flagHelpEnabled = false; // holds whether the help flag can be called during in-program-input

		const char * IPI_flagPrompt = nullptr; // holds the prompts which will be displayed to the user for each ordered input

		const char * IPI_flagHelp = nullptr; // holds the string which the user will see if they ask for help during flag input with ?

		bool IPI_repeatedFinalTestsEnabled = false;



		// array of pointers to bool functions()
		// if one of these tests returns false, display the corresponding error message and throw a "test_failed" exception
		// these are run AFTER input is processed, and will often contain pointers to variables which are modified by input processing.
		// these will be able to take user-defined functions, but are designed to hold the default ones defined in this namespace.
		// as such, they will not be handled in a way that is easy to program for as a user.
		// first arg = called from IPI, second = argc, third = all the main args
		function<bool> ** testFuncs = nullptr;

		bool * testHasFallback = nullptr; // array of bools which hold if another test should be run when the current test fails

		int testCount = 0; // holds the test which is currently being written to before interpreting, and the number of tests set during interpreting



		// this is the main function for setting ordered vars, which is called by the public overloads
		void setOrdered(void * var, void * defaultVal, type t, bool * indicator = nullptr);

		
		void handleFlag(std::string flag, int * i, int argc, std::string * argv);
		void setValue(void * var, std::string in, type t, int index, int flagVar = invalid_input::notAflag);
		void setDefault(void * var, void * def, type t, int index, int flagVar = invalid_input::notAflag);
		void runTests();
		template <class T> T processNumber(std::string in, int index, int flagVar);
		template <class T> tc getTypeConstant();
		template <class... c_strs> void debug_listOutput(std::ostream& out, int unused, c_strs ...orderedVarNames);


		friend invalid_input;
	public:

		void setNumFlags(int n);
		void setNumOrderedVars(int n);
		void setMinOrdered(int n);
		void setNumTests(int n);

		void setEscape(char escape);
		void setFlagDelim(char flagDelim);
		void setFlagVarDefaultMarker(std::string fVarDefaultMarker);
		void setFlagFillDefaultMarker(std::string fFillDefaultMarker);
		void setFlagDefaultMarkers(std::string fVarDefaultMarker, std::string fFillDefaultMarker);

		input_handler(int argc, char * argv[]);
		~input_handler();


		template <class T>
		void setFlag(std::string name, T & var, bool * indicator = nullptr);

		template <class T>
		void setFlag(std::string name, T & var, T defaultVal, bool * indicator = nullptr);
		template <class T>
		void setFlag(std::string name, T & var, function<T> * defaultVal, bool * indicator = nullptr);
		template <class T>
		void setFlag(std::string name, T & var, T * defaultVal, bool * indicator = nullptr);

		template <class T>
		void setFlag(std::string name, T * vars[], int argc, bool * indicator = nullptr);
		void setFlag(std::string name, void * vars[], type types[], int argc, bool * indicator = nullptr);

		template <class T>
		void setFlag(std::string name, T * vars[], T defaultVals[], int argc, bool * indicator = nullptr);
		template <class T>
		void setFlag(std::string name, T * vars[], function<T> * defaultVals[], int argc, bool * indicator = nullptr);
		template <class T>
		void setFlag(std::string name, T * vars[], T * defaultVals[], int argc, bool * indicator = nullptr);

		void setFlag(std::string name, void * vars[], void * defaultVals[], type types[], int argc, bool * indicator = nullptr);

		void setFlag(std::string name, bool * indicator);
		void setFlag(std::string name);

		// pass indicator by reference
		template <class T> void setFlag(std::string name, T & var, bool& indicator) { setFlag(name, var, &indicator); }
		template <class T> void setFlag(std::string name, T & var, T defaultVal, bool& indicator) { setFlag(name, var, defaultVal, &indicator); }
		template <class T> void setFlag(std::string name, T & var, function<T> * defaultVal, bool& indicator) { setFlag(name, var, defaultVal, &indicator); }
		template <class T> void setFlag(std::string name, T & var, T * defaultVal, bool& indicator) { setFlag(name, var, defaultVal, &indicator); }
		template <class T> void setFlag(std::string name, T * vars[], int argc, bool& indicator) { setFlag(name, vars, argc, &indicator); }
		void setFlag(std::string name, void * vars[], type types[], int argc, bool& indicator) { setFlag(name, vars, types, argc, &indicator); }
		template <class T> void setFlag(std::string name, T * vars[], T defaultVals[], int argc, bool& indicator) { setFlag(name, vars, defaultVals, argc, &indicator); }
		template <class T> void setFlag(std::string name, T * vars[], function<T> * defaultVals[], int argc, bool& indicator) { setFlag(name, vars, defaultVals, argc, &indicator); }
		template <class T> void setFlag(std::string name, T * vars[], T * defaultVals[], int argc, bool& indicator) { setFlag(name, vars, defaultVals, argc, &indicator); }
		void setFlag(std::string name, void * vars[], void * defaultVals[], type types[], int argc, bool& indicator) { setFlag(name, vars, defaultVals, types, argc, &indicator); }
		void setFlag(std::string name, bool& indicator) { setFlag(name, &indicator); }



		template <class T>
		void setOrdered(T & var, bool * indicator = nullptr);

		template <class T>
		void setOrdered(T & var, T defaultVal, bool * indicator = nullptr);
		template <class T>
		void setOrdered(T & var, function<T> * defaultVal, bool * indicator = nullptr);
		template <class T>
		void setOrdered(T & var, T * defaultVal, bool * indicator = nullptr);

		// pass indicator by reference
		template <class T> void setOrdered(T & var, bool& indicator) { setOrdered(var, &indicator); }
		template <class T> void setOrdered(T & var, T defaultVal, bool& indicator) { setOrdered(var, defaultVal, &indicator); }
		template <class T> void setOrdered(T & var, function<T> * defaultVal, bool& indicator) { setOrdered(var, defaultVal, &indicator); }
		template <class T> void setOrdered(T & var, T * defaultVal, bool& indicator) { setOrdered(var, defaultVal, &indicator); }



		void setHelpMessage(const char * message);

		void setupDynamicInput(const char * prompts[], const char * flagPrompt = nullptr, const char * flagHelpString = nullptr);
		void setupDynamicInput(const char * prompts[], int tests[], const char * flagPrompt = nullptr, const char * flagHelpString = nullptr);
		void setupDynamicInput(function<std::string> ** prompts, int tests[], const char * flagPrompt = nullptr, const char * flagHelpString = nullptr);
		void setupDynamicInput(function<std::string> ** prompts, const char * flagPrompt = nullptr, const char * flagHelpString = nullptr);
		void setupDynamicInput(const char * prompt = "");
		void dynamicInput_enableRepeatedClosingTests(bool x = true);

		void preventExcessiveInput();

		std::string getPath();
		std::string getProgramName();

		template <class... Ts>
		void setTest(bool testFunc(unsigned int, void**), Ts& ...args) { setTest(false, testFunc, args...); }

		template <class... Ts>
		void setTest(bool enableFallback, bool testFunc(unsigned int, void**), Ts& ...args) { setTest(enableFallback, new function<bool>(testFunc, &args...)); }

		void setTest(bool enableFallback, function<bool> * testFunc);
		void setTest(function<bool> * testFunc);


		void interpretInput(std::ostream& = std::cout, std::istream& = std::cin);


		template <class... c_strs>
		void debug_listOutput(std::ostream& out, c_strs... orderedVarNames);

		template <class... c_strs>
		void debug_listOutput(c_strs... orderedVarNames);

		void debug_listOutput();
	};
}




namespace CIP // for random, helpful things like array constructors or prebuilt functions.
{
	template <class T, class... Ts>
	function<T> * fptr(T func(unsigned int, void**), Ts... args)
	{
		return new function<T>(func, args...);
	}

	template <class T, class... Ts>
	function<T> * fref(T func(unsigned int, void**), Ts&... args)
	{
		return fptr(func, &args...);
	} 

	template <class T, class... Ts>
	T ** tvars(T& var, Ts&... vars)
	{
		return new T*[sizeof...(vars) + 1]{ &var, &vars... };
	}

	template <class T, class... Ts>
	T * defaults(T val, Ts... vals)
	{
		return new T[sizeof...(vals) + 1]{ val, vals... };
	}

	bool stringsArentEqual(unsigned int argc, void* argv[]); // (string name, string name2, ...)
	bool fileExistsAndCanBeOpened(unsigned int argc, void* argv[]); // (string fileName)
	bool NOT(unsigned int argc, void* argv[]); // (bool(*function)(bool,int,void**), args...) 
	bool NOTF(unsigned int argc, void* argv[]); // (function<bool>) 
	bool yesno(unsigned int argc, void* argv[]); // (ostream& output, istream& input, function<string> * prompt, (optional) bool biasTowards)

	template <class... strings>
	function<bool> * STRINGS_ARENT_EQUAL(std::string& name1, std::string& name2, strings&... names)
	{
		return fref(stringsArentEqual, name1, name2, names...);
	}

	function<bool> * FILE_EXISTS_AND_CAN_BE_OPENED(std::string& fileName);
	function<bool> * YESNO(std::ostream& output, std::istream& input, function<std::string> * prompt, bool biasTowards);
	function<bool> * YESNO(std::ostream& output, std::istream& input, function<std::string> * prompt);
	function<bool> * YESNO(std::ostream& output, std::istream& input, function<std::string> * prompt);


	std::string strFsum(unsigned int argc, void** argv); // (function<string>, function<string>)
	std::string namePlus(unsigned int argc, void** argv); // (string originalString, const char * suffix) (string originalString, const char * prefix, const char * suffix)
	std::string namePlusFunc(unsigned int argc, void** argv); // (string originalString, string function array suffix) or (string originalString, string function array prefix, string function array suffix)
	std::string fileExtension(unsigned int argc, void** argv); // (string fileName, char extensionDelim = '.', char pathDelim = '\\', bool includeDelim = true)
	std::string noFileExtension(unsigned int argc, void** argv); // (string fileName, char extensionDelim = '.', char pathDelim = '\\', bool includeDelim = false)
	

	function<std::string> * STRfSUM(function<std::string> *, function<std::string> *);
	function<std::string> * NAME_PLUS(std::string& originalString, const char * suffix); 
	function<std::string> * NAME_PLUS(std::string& originalString, const char * prefix, const char * suffix);
	function<std::string> * NAME_PLUS_FUNC(std::string& originalString, function<std::string> * suffix); 
	function<std::string> * NAME_PLUS_FUNC(std::string& originalString, function<std::string> * prefix, function<std::string> * suffix);
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim, bool includeDelim);
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim);
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim);
	function<std::string> * FILE_EXTENSION(std::string& fileName);
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim, bool excludeDelim);
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim);
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim);
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName);
	





	const char * cstr(unsigned int, void**);
	std::string str(unsigned int, void**);
	std::string ternary(unsigned int argc, void** argv);
	std::string ternaryF(unsigned int argc, void** argv);
	std::string Fternary(unsigned int argc, void** argv);
	std::string FternaryF(unsigned int argc, void** argv);
	std::string choose(unsigned int argc, void** argv);
	std::string chooseF(unsigned int argc, void** argv);
	std::string Fchoose(unsigned int argc, void** argv);
	std::string FchooseF(unsigned int argc, void** argv);
	function<const char *> * CSTR(const char *);
	function<std::string> * STR(const char *);
	function<std::string> * TERNARY(bool& condition, const char * trueStr, const char * falseStr);
	function<std::string> * TERNARYf(bool& condition, function<std::string> * trueStr, function<std::string> * falseStr);
	function<std::string> * fTERNARY(function<bool> * condition, const char * trueStr, const char * falseStr);
	function<std::string> * fTERNARYf(function<bool> * condition, function<std::string> * trueStr, function<std::string> * falseStr);

	template <class... c_strs>
	function<std::string> * CHOOSE(unsigned int& index, c_strs... returnVals)
	{
		return fptr(choose, &index, returnVals...);
	}
	template <class... str_funcs>
	function<std::string> * CHOOSEf(unsigned int& index, str_funcs... returnVals)
	{
		return fptr(chooseF, &index, returnVals...);
	}
	template <class... c_strs>
	function<std::string> * fCHOOSE(function<std::string> * index, c_strs... returnVals)
	{
		return fptr(Fchoose, index, returnVals...);
	}
	template <class... str_funcs>
	function<std::string> * fCHOOSEf(function<std::string> * index, str_funcs... returnVals)
	{
		return fptr(FchooseF, index, returnVals...);
	}


	template <class... Typs>
	type * types(Typs... ts)
	{
		return new type[sizeof...(ts)]{ ts... };
	}

	type * sameType(type t, unsigned int count);

	template <class... Ts>
	void ** vars(Ts&... vars)
	{
		return new void*[sizeof...(vars)] { &vars... };
	}

	template <class... ints>
	int * tests(ints... vars)
	{
		return new int[sizeof...(vars)]{ vars... };
	}

	template <class T, size_t numElements>
	T ** arrayVar(T (&ary)[numElements])
	{
		T ** result = new T*[numElements];
		for (unsigned int i = 0; i < numElements; ++i)
			result[i] = &ary[i]; // ...this is an awful construct.
		return result;
	}
}


















namespace CIP // for error-type function template definitions
{
	template <class... T>
	invalid_input::invalid_input(input_handler& IH, bool type, int index, int subIndex, std::string problem, T... problemArgs)
	{
		this->IH = &IH;
		this->type = type;
		this->index = type == ordered ? index : subIndex;
		if (type == flag)
			name = IH.flags[index];
		else
		{
			int bytes = snprintf(NULL, 0, "Ordered Variable %i", index + 1);
			char * buf = new char[bytes + 1];
			snprintf(buf, bytes + 1, "Ordered Variable %i", index + 1);
			name = buf;
			delete[] buf;
		}
		int bytes = snprintf(NULL, 0, problem.c_str(), problemArgs...);
		char * buf = new char[bytes + 1];
		snprintf(buf, bytes + 1, problem.c_str(), problemArgs...);
		this->problem = buf;
		delete[] buf;
	}

	template <class... T>
	invalid_input::invalid_input(input_handler& IH, int index, std::string problem, T... problemArgs)
	{
		*this = invalid_input(IH, ordered, index, 0, problem, problemArgs...);
	}

	template <class... T>
	invalid_input::invalid_input(input_handler& IH, int index, int subIndex, std::string problem, T... problemArgs)
	{
		if (subIndex == notAflag)
			*this = invalid_input(IH, ordered, index, 0, problem, problemArgs...);
		else
			*this = invalid_input(IH, flag, index, subIndex, problem, problemArgs...);
	}
}








namespace CIP // for <type>str function template definitions
{
	template<class... T>
	void boolstr::setMarkersFor(bool cstringsEqual, T... cstrings)
	{
		delete[] *strings[cstringsEqual]; // strings is pointing at the address of the pointer which is pointing at a list of strings.
		*numStrings[cstringsEqual] = sizeof...(cstrings);
		if (numStrings > 0)
			*strings[cstringsEqual] = new const char*[*numStrings[cstringsEqual]]{ cstrings... };
		else
			*strings[cstringsEqual] = nullptr;
	}

	template<class... T>
	inline boolstr::boolstr(bool & var, bool cstringsEqual, T... cstrings)
	{
		this->var = &var;
		setMarkersFor(cstringsEqual, cstrings...);
	}

	template<class... T>
	intstr::intstr(int & var, int cstringsEqual, T... cstrings)
	{
		this->var = &var;
		setMarkersFor(cstringsEqual, cstrings...);
		lengthOfLongestCorrespondsToValue = lengthOfValue(cstringsEqual);
	}

	template<class... T>
	void intstr::setMarkersFor(bool cstringsEqual, T... cstrings)
	{
		if (lengthOfLongestCorrespondsToValue < lengthOfValue(cstringsEqual))
			lengthOfLongestCorrespondsToValue = lengthOfValue(cstringsEqual);
		unsigned int index = exists(cstringsEqual);
		unsigned int numStrings = sizeof...(cstrings);
		if (index != -1)
		{
			delete[] data[index].string;
		}
		else
		{
			index = numData++;
			Data * temp = new Data[numData];
			for (int i = 0; i < index; ++i)
				temp[i] = data[i];
			delete[] data;
			data = temp;
		}
		Data& d = data[index];
		d.correspondsTo = cstringsEqual;
		d.numStrings = numStrings;
		d.string = new char*[numStrings] {cstrings...};
	}
}




namespace CIP // for input_handler function template definitions
{
	template <class T>
	T input_handler::processNumber(std::string in, int index, int flagVar) try
	{
#ifndef CIP_NO_FLOATING_POINT_INPUTS
#ifndef CIP_NO_FLOAT_INPUTS
		static_assert(numeric_limits<float>::has_infinity, "variables of type \"float\" will not be read correctly by input_handler for this target platform."\
			" #define CIP_NO_FLOAT_INPUTS to supress this error.");
#endif
#ifndef CIP_NO_DOUBLE_INPUTS
		static_assert(numeric_limits<float>::has_infinity, "variables of type \"double\" will not be read correctly by input_handler for this target platform."\
			" #define CIP_NO_DOUBLE_INPUTS to supress this error.");
#endif
#ifndef CIP_NO_LONG_DOUBLE_INPUTS
		static_assert(numeric_limits<float>::has_infinity, "variables of type \"long double\" will not be read correctly by input_handler for this target platform."\
			" #define CIP_NO_LONG_DOUBLE_INPUTS to supress this error.");
#endif
#endif

		if (numeric_limits<T>::has_infinity)
		{
			return (T)stold(in);
		}
		else if (numeric_limits<T>::is_signed)
		{
			signed long long result = stoll(in);
#pragma	warning(push)
#pragma warning(disable: 4018)
			if (result > numeric_limits<T>::max())
			{
				throw invalid_input(*this, index, flagVar, "Overflow error (an input of %i was provided to a varaible that can hold a maximum value of %i)", result, numeric_limits<T>::max());
			}
			else if (result < numeric_limits<T>::min())
			{
				throw invalid_input(*this, index, flagVar, "Underflow error (an input of %i was provided to a varaible that can hold a minimum value of %i)", result, numeric_limits<T>::min());
			}
			else
				return (T)result;
#pragma warning(pop)
		}
		else
		{
			int i;
			for (i = 0; isspace(static_cast<unsigned char>(in[i])); ++i);
			if (in[i] == '-')
			{
				throw invalid_input(*this, index, flagVar, "Sign error (a negative input was provided to an unsigned integral variable)");
			}
			unsigned long long result = stoull(in);
			if (result > (unsigned long long)numeric_limits<T>::max())
			{
				throw invalid_input(*this, index, flagVar, "Overflow error (an input of %i was provided to a varaible that can hold a maximum value of %i)", result, numeric_limits<T>::max());
			}
			else
				return (T)result;
		}
	}/*
	template <class... T>
	invalid_input(input_handler& IH, int index, std::string problem, T... problemArgs);

	template <class... T>
	invalid_input(input_handler& IH, int index, int subIndex, std::string problem, T... problemArgs);*/
	catch(std::invalid_argument) {
		throw invalid_input(*this, index, flagVar, "Input '%s' cannot be converted to %s type.", in.c_str(), (getTypeConstant<T>() >= TYPE::DIVIDERS::floatingTypeStart) ? "a floating point" : "an integral");
	}


	template <class T> tc input_handler::getTypeConstant() { throw programming_error("an inalid <type> parameter was provided to a function"); }
	// these declared specialisations are defined in Console Input Parser.cpp

#define helper(t) template<> tc input_handler::getTypeConstant<t>();
#define Declare(t)						\
	helper(t)							\
	helper(t(*)(unsigned int,void**)) 	\
	helper(function<t>)					\
	helper(function<t>*)				\
	helper(t*)

	Declare(std::string)
	Declare(char)
	Declare(unsigned char)
	Declare(short)
	Declare(unsigned short)
	Declare(int)
	Declare(unsigned int)
	Declare(long)
	Declare(unsigned long)
	Declare(long long)
	Declare(unsigned long long)
	Declare(float)
	Declare(double)
	Declare(long double)
	Declare(bool)
	Declare(boolstr)
	Declare(intstr)

#undef helper
#undef Declare

	 // main setFlag func:
	// void input_handler::setFlag(string name, void * vars[], void * defaultVals[], type types[], int argc, bool * indicator)
	template <class T>
	void input_handler::setFlag(std::string name, T & var, bool * indicator)
	{
		T ** vars = new T*[1]{ &var };
		setFlag(name, vars, 1, indicator);
	}


#define shared_code(defT) \
	T ** vars = new T*[1]{ &var }; \
	defT * defaultVals = new defT[1]{ defaultVal }; \
	setFlag(name, vars, defaultVals, 1, indicator)

	template <class T>
	void input_handler::setFlag(std::string name, T & var, T defaultVal, bool * indicator)
	{
		shared_code(T);
	}
	template <class T>
	void input_handler::setFlag(std::string name, T & var, function<T> * defaultVal, bool * indicator)
	{ 
		shared_code(function<T>*);
	}
	template <class T>
	void input_handler::setFlag(std::string name, T & var, T * defaultVal, bool * indicator)
	{
		shared_code(T*);
	}

#undef shared_code

#define makeVoidVars						\
	void ** voidVars = new void*[argc];	   	\
		for (int i = 0; i < argc; ++i)		\
			voidVars[i] = vars[i];			\
	delete[] vars								  

	template <class T>
	void input_handler::setFlag(std::string name, T * vars[], int argc, bool * indicator)
	{
		makeVoidVars;
		setFlag(name, voidVars, nullptr, sameType(getTypeConstant<T>(), argc), argc, indicator);
	}

#define defByAdr							\
	void ** voidDefault = new void*[argc];	\
	for (int i = 0; i < argc; ++i)		   	\
		voidDefault[i] = &defaultVals[i]	
											
#define defByVal							\
	void ** voidDefault = new void*[argc];	\
	for (int i = 0; i < argc; ++i)		   	\
		voidDefault[i] = defaultVals[i];	\
	delete[] defaultVals

#define shared_code(ty,def)				   		  \
	makeVoidVars;								  \
	def;										  \
	type * types = new type[argc];		   		  \
	for (int i = 0; i < argc; ++i)		   		  \
		types[i] = getTypeConstant<ty>();		  \
	setFlag(name, voidVars, voidDefault, types, argc, indicator)
	
	template <class T>
	void input_handler::setFlag(std::string name, T * vars[], T defaultVals[], int argc, bool * indicator)
	{
		shared_code(T, defByAdr);
	}
	template <class T>
	void input_handler::setFlag(std::string name, T * vars[], function<T> * defaultVals[], int argc, bool * indicator)
	{
		shared_code(function<T>, defByVal);
	}
	template <class T>
	void input_handler::setFlag(std::string name, T * vars[], T * defaultVals[], int argc, bool * indicator)
	{
		shared_code(T*, defByAdr);
	}

#undef makeVoidVars
#undef shared_code



	 // main setOrdered func:
	// void input_handler::setOrdered(void * var, void * defaultVal, type t, bool * indicator)

	template<class T>
	inline void input_handler::setOrdered(T & var, bool * indicator)
	{
		setOrdered(&var, nullptr, getTypeConstant<T>(), indicator);
	}

	template<class T> 
	inline void input_handler::setOrdered(T & var, T defaultVal, bool * indicator)
	{
		void * defVal = new T;
		*static_cast<T*>(defVal) = defaultVal;
		setOrdered(&var, defVal, getTypeConstant<T>(), indicator);
	}
	template<class T> 
	inline void input_handler::setOrdered(T & var, function<T> * defaultVal, bool * indicator)
	{
		setOrdered(&var, defaultVal, getTypeConstant<function<T>>(), indicator);
	}
	template<class T> 
	inline void input_handler::setOrdered(T & var, T * defaultVal, bool * indicator)
	{
		void * defVal = new T*;
		*static_cast<T**>(defVal) = defaultVal;
		setOrdered(&var, defVal, getTypeConstant<T*>(), indicator);
	}




#define out_help_case(firststr,var,laschr,t,ty) case t: out << (firststr) << *(ty*)var << (laschr); break
#define out_help_switch(fs,var,lc)											\
			out_help_case(fs, var, lc, STRING, string);						\
			out_help_case(fs, var, lc, CHAR, char);							\
			out_help_case(fs, var, lc, UCHAR, unsigned char);				\
			out_help_case(fs, var, lc, SHORT, short);						\
			out_help_case(fs, var, lc, USHORT, unsigned short);				\
			out_help_case(fs, var, lc, INT, int);							\
			out_help_case(fs, var, lc, UINT, unsigned int);					\
			out_help_case(fs, var, lc, LONG, long);							\
			out_help_case(fs, var, lc, ULONG, unsigned long);				\
			out_help_case(fs, var, lc, LONGLONG, long long);				\
			out_help_case(fs, var, lc, ULONGLONG, unsigned long long);		\
			out_help_case(fs, var, lc, FLOAT, float);						\
			out_help_case(fs, var, lc, DOUBLE, double);						\
			out_help_case(fs, var, lc, LONG_DOUBLE, long double);			\
			out_help_case(fs, var, lc, BOOL, bool);							\
			out_help_case(fs, var, lc, BOOLSTR, boolstr);					\
			out_help_case(fs, var, lc, INTSTR, intstr);

#define output(var,t,initialstr,spacechrs) do{\
		switch (t % func_startVal)\
		{\
			using namespace TYPE;\
			out_help_switch(initialstr,var,spacechrs);\
		} } while (false)


	template <class... c_strs>
	void input_handler::debug_listOutput(std::ostream& out, int unused, c_strs... orderedVarNames)
	{
		using namespace CIP::TYPE::DIVIDERS;
		using namespace std;

		int stringCount = sizeof...(orderedVarNames);
		va_list list;
		va_start(list, unused);

		auto coutStartState = out.flags(); // Only used when out is std::cout.

		if (&out == &cout)
			out.setf(ios_base::boolalpha);

		string * orderedStrings = new string[orderedCount];
		out << "\n\n\tOrdered Variables:\n\n";
		unsigned int longestOrderedStringLength = 0;
		for (int i = 0; i < orderedCount; ++i)
		{
			orderedStrings[i] = "\t\t" + (i < stringCount ? va_arg(list, const char*) : "Ordered var " + to_string(i));
			if (orderedStrings[i].length() > longestOrderedStringLength)
				longestOrderedStringLength = orderedStrings[i].length();
		}
		for (int i = 0; i < orderedCount; ++i)
		{
			string preSpaces = "";
			for (unsigned int j = 0; j < longestOrderedStringLength - orderedStrings[i].length(); ++j)
				preSpaces += " ";
			orderedStrings[i].insert(2, preSpaces);
		}


		for (int i = 0; i < orderedCount; ++i)
		{
			out << orderedStrings[i];
			if (*orderedSet[i])
				output(orderedVars[i], orderedTypes[i], ": ", '\n');
			else if (orderedHasDefault)
				output(orderedVars[i], orderedTypes[i], " (", ")\n");
			else
				out << '*';
		}
		out << "\n\tSet Flags:\n";
		for (int i = 0; i < flagCount; ++i)
		{
			if (*flagSet[i])
			{
				out << "\n\t\t" << flags[i] << (flagArgCounts[i] > 0 ? " = " : "");
				for (int t = 0; t < flagArgCounts[i]; ++t)
					output(flagVars[i][t], flagTypes[i][t],"", ", ");
				out << "\b\b ";
			}
			while (i + 1 < flagCount && flagIsAlias[i + 1])
				++i;
		}
		out << "\n\n\tUnset Flags:\n\n";
		for (int i = 0; i < flagCount; ++i)
			if (!(*flagSet[i] || flagIsAlias[i]))
			{
				out << "\t\t" << flags[i];
				if (flagHasDefault[i])
				{
					out << " (";
					for (int t = 0; t < flagArgCounts[i]; ++t)
						output(flagVars[i][t], flagTypes[i][t],"", ", ");
					out << "\b\b)";
				}
				out << '\n';
			}

		if (&out == &cout)
			out.flags(coutStartState);

		delete[] orderedStrings;
	}

	template <class... c_strs>
	void debug_listOutput(c_strs... orderedVarNames)
	{
		debug_listOutput(std::cout, orderedVarNames...);
	}

	template <class... c_strs>
	void input_handler::debug_listOutput(std::ostream& out, c_strs... orderedVarNames)
	{
		debug_listOutput(out, 0, orderedVarNames...);
	}

#undef output
#undef out_help_s
#undef out_help_case
}