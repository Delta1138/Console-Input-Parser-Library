#include "pch.h"
#include "framework.h"
#include "CIP.h"
#include <fstream>

using namespace std;
#define FILE_AND_LINE (__FILE__ "(" + to_string(__LINE__) + ")")

namespace CIP
{
	bool stringsArentEqual(unsigned int argc, void * argv[]) // (string name, string name2, ...)
	{
		if (argc < 2)
			throw programming_error("stringsArentEqual() must be provided at least two strings");
		for (unsigned int i = 0; i < argc - 1; ++i)
			for (unsigned int j = i + 1; j < argc; ++j)
				if (*(string*)argv[i] == *(string*)argv[j])
					return false;
		return true;
	}

	bool fileExistsAndCanBeOpened(unsigned int argc, void * argv[])
	{
		if (argc != 1)
			throw programming_error("fileExistsAndCanBeOpened() must be provided exactly one argument, which must be a std::string containing a filename.");
		ofstream temp;
		temp.open(*(string*)argv[0]);
		if (temp.is_open())
		{
			temp.close();
			return true;
		}
		else
			return false;
	}

	bool NOTF(unsigned int argc, void * argv[])
	{
		if (argc != 1)
			throw programming_error("NOTF() takes 1 argument, a function<bool>");

		return !(static_cast<function<bool>*>(argv[0])->value());
	}

	bool NOT(unsigned int argc, void * argv[])
	{
		if (argc == 0)
			throw programming_error("NOT() must receive at least one argument, the function it is to evaluate");
		void ** newArgv = nullptr;
		auto newArgc = argc - 1;
		unsigned int * deleteArgOnDestruction = nullptr;
		type * typesOfArgsToDelete = nullptr;

		for (unsigned int i = 1; i < argc; ++i)
			if (argv[i] == nullptr)
			{
				for (void * temp = nullptr; temp == nullptr && i < argc; temp = argv[0], ++i)
					--newArgc;
				--newArgc;
			}

		if (newArgc > 0)
		{
			newArgv = new void*[newArgc];
			deleteArgOnDestruction = new unsigned int[newArgc] {0};
			typesOfArgsToDelete = new type[newArgc + 1]{ 0 };
			for (unsigned int i = 1, arg = 0; i < argc; ++i)
			{
				void * temp = argv[i];
				if (temp)
				{
					newArgv[arg] = temp;
					++arg;
				}
				else
				{
					while (!temp && i < argc)
					{
						++deleteArgOnDestruction[arg];
						++i;
						temp = argv[i];
					}
					if (temp)
					{
						typesOfArgsToDelete[arg] = *(type*)temp;
						delete temp;
					}
				}
			}
		}
		return function<bool>(static_cast<bool(*)(unsigned int, void**)>(argv[0]), newArgc, newArgv, deleteArgOnDestruction, typesOfArgsToDelete).value();
	}

	bool yesno(unsigned int argc, void * argv[]) 
	{
		if (argc < 3 || argc > 4)
			throw programming_error("yesno()'s args are (ostream& output, istream& input, function<string> * prompt, (optional) bool biasTowards)");

		ostream& os = *static_cast<ostream*>(argv[0]);
		istream& is = *static_cast<istream*>(argv[1]);
		function<string>& prompt = *static_cast<function<string>*>(argv[2]);

	tryAgain:
		char in;
		os << prompt;
		is >> in;

		if (in == 'y' || in == 'Y')
			return true;
		else if (in == 'n' || in == 'N')
			return false;
		else if (argc == 4) // biasTowards was set and is argv[3]
			return *static_cast<bool*>(argv[3]);
		else
		{
			os << "Input either y or n.\n";
			goto tryAgain;
		}
	}

	string strFsum(unsigned int argc, void ** argv) // (string function array, string function array)
	{
		if (argc < 2)
			throw programming_error("strFsum()'s args are (function<string>, function<string>, ...)");
		string sum = "";
		for (unsigned int i = 0; i < argc; ++i)
			sum += static_cast<function<string>*>(argv[i])->value();
		return sum;
	}

	string namePlus(unsigned int argc, void ** argv)
	{
		if (argc < 2 || argc > 3)
			throw programming_error("namePlus()'s args are (string originalString, const char * suffix) or (string originalString, const char * prefix, const char * suffix)");
		string& originalString = *static_cast<string*>(argv[0]);

		const char * suffix;
		const char * prefix = "";
		if (argc == 3)
		{
			prefix = static_cast<const char *>(argv[1]);
			suffix = static_cast<const char *>(argv[2]);
		}
		else
			suffix = static_cast<const char *>(argv[1]);

		return prefix + originalString + suffix;
	}

	string namePlusFunc(unsigned int argc, void ** argv) 
	{
		if (argc < 2 || argc > 3)
			throw programming_error("namePlusFunc()'s args are (string originalString, function<string> suffix) or (string originalString, function<string> prefix, function<string> suffix)");
		string& originalString = *static_cast<string*>(argv[0]);

		string suffix;
		string prefix = "";
		if (argc == 2)
			suffix = static_cast<function<string>*>(argv[1])->value();
		else
		{
			prefix = static_cast<function<string>*>(argv[1])->value();
			suffix = static_cast<function<string>*>(argv[2])->value();
		}

		return prefix + originalString + suffix;
	}

	string fileExtension(unsigned int argc, void ** argv) 
	{
		if (argc < 1 || argc > 4)
			throw programming_error("fileExtension()'s args are (string filename, bool includeDelim = true, char * pathDelims = \"/\\\\\", char extensionDelim = '.')");

		string& fileName = *static_cast<string*>(argv[0]);

		bool excludeDelim = false;
		if (argc > 1)
			excludeDelim = !*static_cast<bool*>(argv[1]);

		string pathDelims = "/\\";
		if (argc > 2)
			pathDelims = static_cast<char*>(argv[2]);

		char extensionDelim = '.';
		if (argc > 3)
			extensionDelim = *static_cast<char*>(argv[3]);

		size_t i = fileName.find_last_of(pathDelims + extensionDelim);
		if (i != string::npos && fileName[i] != extensionDelim)
			return "";
		else
			return fileName.substr(i + excludeDelim);
	}

	string noFileExtension(unsigned int argc, void ** argv)
	{
		if (argc < 1 || argc > 4)
			throw programming_error("noFileExtension()'s args are (string filename, bool includeDelim = false, char * pathDelims = \"/\\\\\", char extensionDelim = '.')");

		string& fileName = *static_cast<string*>(argv[0]);

		bool includeDelim = false;
		if (argc > 1)
			includeDelim = *static_cast<bool*>(argv[1]);

		string pathDelims = "/\\";
		if (argc > 2)
			pathDelims = static_cast<char*>(argv[2]);

		char extensionDelim = '.';
		if (argc > 3)
			extensionDelim = *static_cast<char*>(argv[3]);

		size_t i = fileName.find_last_of(pathDelims + extensionDelim);
		if (i != string::npos && fileName[i] != extensionDelim)
			return fileName;
		else
			return fileName.substr(0, i + includeDelim);
	}

	string ternary(unsigned int argc, void ** argv) // (bool condition, const char * trueStr, const char * falseStr)
	{
		if (argc != 3)
			throw programming_error("ternary()'s args are (bool * condition, const char * trueStr, const char * falseStr)");

		if (*(bool*)argv[0])
			return (const char *)argv[1];
		else
			return (const char *)argv[2];
	}

	std::string choose(unsigned int argc, void ** argv)
	{
		if (argc < 2)
			throw programming_error("choose() requires at least two arguments, an unsigned int and at least one possible results-string");
		unsigned int index = *(unsigned int*)(argv[0]);
		++index; // since index takes up 1 spot in the array
		if (index >= argc)
			throw programming_error(("choose()'s first argument was " + to_string(index) + ", but it only recieved " + to_string(argc) + "arguments.").c_str());
		return (const char*)(argv[index]);
	}

	std::string chooseF(unsigned int argc, void ** argv)
	{
		if (argc < 2)
			throw programming_error("chooseF() requires at least two arguments, an unsigned int and at least one possible results-string");
		unsigned int index = *(unsigned int*)(argv[0]);
		++index; // since index takes up 1 spot in the array
		if (index >= argc)
			throw programming_error(("chooseF()'s first argument was " + to_string(index) + ", but it only recieved " + to_string(argc) + "arguments.").c_str());
		return ((function<string>*)(argv[index]))->value();
	}

	std::string Fchoose(unsigned int argc, void ** argv)
	{
		if (argc < 2)
			throw programming_error("Fchoose() requires at least two arguments, an unsigned int and at least one possible results-string");
		unsigned int index = ((function<unsigned int>*)(argv[0]))->value();
		++index; // since index takes up 1 spot in the array
		if (index >= argc)
			throw programming_error(("Fchoose()'s first argument was " + to_string(index) + ", but it only recieved " + to_string(argc) + "arguments.").c_str());
		return (const char*)(argv[index]);
	}

	std::string FchooseF(unsigned int argc, void ** argv)
	{
		if (argc < 2)
			throw programming_error("FchooseF() requires at least two arguments, an unsigned int and at least one possible results-string");
		unsigned int index = ((function<unsigned int>*)(argv[0]))->value();
		++index; // since index takes up 1 spot in the array
		if (index >= argc)
			throw programming_error(("FchooseF()'s first argument was " + to_string(index) + ", but it only recieved " + to_string(argc) + "arguments.").c_str());
		return ((function<string>*)(argv[index]))->value();
	}

	string ternaryF(unsigned int argc, void ** argv) // (bool condition, function<string> trueStr, function<string> falseStr)
	{
		if (argc != 3)
			throw programming_error("ternaryF()'s args are (bool condition, function<string> trueStr, function<string> falseStr)");

		int x = 2 - *(bool*)argv[0];

		return static_cast<function<string>*>(argv[x])->value();
	}

	const char * cstr(unsigned int x, void ** v) // (const char * str)
	{
		if (x != 1)
			throw programming_error("cstr must recieve exactly 1 argument, the const char * it is to return");
		return (const char *)(*v);
	}

	string str(unsigned int x, void ** v) // (const char * str)
	{
		if (x != 1)
			throw programming_error("str must recieve exactly 1 argument, the const char * it is to return");
		return string((const char *)(*v));
	}

	std::string Fternary(unsigned int argc, void ** argv)
	{
		if (argc != 3)
			throw programming_error("Fternary()'s args are (function<bool> condition, const char * trueStr, const char * falseStr)");

		if (static_cast<function<bool>*>(argv[0])->value())
			return (const char *)argv[1];
		else
			return (const char *)argv[2];
	}

	std::string FternaryF(unsigned int argc, void ** argv)
	{
		if (argc != 3)
			throw programming_error("Fternary()'s args are (function<bool> condition, function<string> trueStr, function<string> falseStr)");

		if (static_cast<function<bool>*>(argv[0])->value())
			return static_cast<function<string>*>(argv[1])->value();
		else
			return static_cast<function<string>*>(argv[2])->value();
	}

	using namespace TYPE;

	type * CIP::sameType(type t, unsigned int count)
	{
		type * res = nullptr;
		if(count != 0)
			res = new type[count];
		for (unsigned int i = 0; i < count; ++i)
			res[i] = t;
		return res;
	}

#define ty new type

	function<const char *> * CSTR(const char * x)
	{
		return fptr(cstr, x);
	}
	function<std::string> * STR(const char * x)
	{
		return fptr(str, x);
	}
	function<std::string> * TERNARY(bool& condition, const char * trueStr, const char * falseStr)
	{
		return fptr(ternary, &condition, trueStr, falseStr);
	}
	function<std::string> * TERNARYf(bool& condition, function<string> * trueStr, function<string> * falseStr)
	{
		return fptr(ternaryF, &condition, nullptr, ty{ FUNC::STR }, trueStr, nullptr, ty{ FUNC::STR }, falseStr);
	}
	function<std::string> * fTERNARY(function<bool> * condition, const char * trueStr, const char * falseStr)
	{
		return fptr(Fternary, nullptr, ty{ FUNC::STR }, condition, trueStr, falseStr);
	}
	function<std::string> * fTERNARYf(function<bool> * condition, function<string> * trueStr, function<string> * falseStr)
	{
		return fptr(FternaryF, nullptr, ty{ FUNC::STR }, condition, nullptr, ty{ FUNC::STR }, trueStr, nullptr, ty{ FUNC::STR }, falseStr);
	}
	function<bool> * FILE_EXISTS_AND_CAN_BE_OPENED(std::string& fileName)
	{
		return fref(fileExistsAndCanBeOpened, fileName);
	}
	function<bool> * YESNO(ostream& output, istream& input, function<string> * prompt, bool biasTowards)
	{
		return fptr(yesno, &output, &input, nullptr, ty{ FUNC::STR }, prompt, nullptr, ty{ BOOL }, new bool{ biasTowards });
	}
	function<bool> * YESNO(ostream& output, istream& input, function<string> * prompt)
	{
		return fptr(yesno, &output, &input, nullptr, ty{ FUNC::STR }, prompt);
	}
	function<string> * STRfSUM(function<string> * func1, function<string> * func2)
	{
		return fptr(strFsum, nullptr, ty{ FUNC::STR }, func1, nullptr, ty{ FUNC::STR }, func2);
	}
	function<string> * NAME_PLUS(string& originalString, const char * suffix)
	{
		return fptr(namePlus, &originalString, suffix);
	}
	function<string> * NAME_PLUS(string& originalString, const char * prefix, const char * suffix)
	{
		return fptr(namePlus, &originalString, prefix, suffix);
	}
	function<string> * NAME_PLUS_FUNC(string& originalString, function<string> * suffix)
	{
		return fptr(namePlusFunc, &originalString, nullptr, ty{ FUNC::STR }, suffix);
	}
	function<string> * NAME_PLUS_FUNC(string& originalString, function<string> * prefix, function<string> * suffix)
	{
		return fptr(namePlus, &originalString, nullptr, ty{ FUNC::STR }, prefix, nullptr, ty{ FUNC::STR }, suffix);
	}
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim, bool includeDelim)
	{
		return fptr(fileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim }, nullptr, ty{ CHAR }, new char{ pathDelim }, nullptr, ty{ BOOL }, new bool{ includeDelim });
	}
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim)
	{
		return fptr(fileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim }, nullptr, ty{ CHAR }, new char{ pathDelim });
	}
	function<std::string> * FILE_EXTENSION(std::string& fileName, char extensionDelim)
	{
		return fptr(fileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim });
	}
	function<std::string> * FILE_EXTENSION(std::string& fileName)
	{
		return fptr(fileExtension, &fileName);
	}
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim, bool excludeDelim)
	{
		return fptr(noFileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim }, nullptr, ty{ CHAR }, new char{ pathDelim }, nullptr, ty{ BOOL }, new bool{ excludeDelim });
	}
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim, char pathDelim)
	{
		return fptr(noFileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim }, nullptr, ty{ CHAR }, char{ pathDelim });
	}
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName, char extensionDelim)
	{
		return fptr(noFileExtension, &fileName, nullptr, ty{ CHAR }, new char{ extensionDelim });
	}
	function<std::string> * NO_FILE_EXTENSION(std::string& fileName)
	{
		return fptr(noFileExtension, &fileName);
	}
}