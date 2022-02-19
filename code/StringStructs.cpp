#include <iostream>
#include "pch.h"
#include "framework.h"
#include "CIP.h"

using namespace std;
#define FILE_AND_LINE (__FILE__ "(" + to_string(__LINE__) + ")")

namespace CIP
{
	// BOOLSTR
	
	ostream& operator<<(ostream &x, boolstr &y) { return x << *y.var; }

	boolstr::~boolstr()
	{
		delete[] trueStrings;
		delete[] falseStrings;
	}

	
	void boolstr::assignValue(string input, invalid_input& exception) { assignValue(input, &exception); }

	void boolstr::assignValue(string input, invalid_input * exception) // TODO: allow boolstr and intstr to ignore capitalization on request (if I ever feel like dealing with human language nonsense)
	{
		if (trueStrings != nullptr)
		{
			for (int i = 0; i < numTrueStrings; ++i)
				if (trueStrings[i] == input)
				{
					*var = true;
					return;
				}
			if (falseStrings == nullptr)
			{
				*var = false;
				return;
			}
			else
			{
				for (int i = 0; i < numFalseStrings; ++i)
					if (falseStrings[i] == input)
					{
						*var = false;
						return;
					}

				/* if (no string matches) */
				{
					string problem;

					string problemPostName = " was not provided a valid string.\n  TRUE: ";
					if(numTrueStrings > 0)
						for (int i = 0; i < numTrueStrings; ++i)
							problemPostName = problemPostName + trueStrings[i] + " ";
					else
						problemPostName += "(anything which does not match false)";
					problemPostName += "\n FALSE: ";

					if (numTrueStrings > 0)
						for (int i = 0; i < numFalseStrings; ++i)
							problemPostName = problemPostName + falseStrings[i] + " ";
					else
						problemPostName += "(anything which does not match true)";

					if (!exception)
						throw runtime_error("A CIP::boolstr " + problemPostName);

					if (exception->type == exception->flag)
						problem = "Flag '" + exception->name + "' argument " + to_string(exception->index + 1) + problemPostName;
					else
						problem = exception->name + problemPostName;

					exception->problem = problem;

					throw *exception;
				}
			}
		}
		else if (falseStrings != nullptr)
		{
			for (int i = 0; i < numFalseStrings; ++i)
				if (falseStrings[i] == input)
				{
					*var = false;
					return;
				}
			*var = true;
			return;
		}
		else
			throw programming_error("boolstr::assignValue was run on a boolstr with neither trueStrings nor falseStrings set");
	}

	// INTSTR

	ostream& operator<<(ostream &x, intstr &y) { return x << *y.var; }

	int intstr::exists(int x)
	{
		for (unsigned int i = 0; i < numData; ++i)
			if (data[i].correspondsTo == x)
				return i;
		return -1;
	}

	int intstr::lengthOfValue(int x)
	{
#ifndef CIP_NO_INTSTRS
		static_assert(11 / 10 == 1 && 19 / 10 == 1 && -11 / 10 == -1 && -19 / 10 == -1, "CIP::intstr not work properly if integer devision does not round to 0 "\
			"(rounding towards 0 is guarenteed as of C++11, but was implementation-defined previously)\n"\
			"#define CIP_NO_INTSTRS to supress this error");
#endif CIP_NO_INTSTRS
		int result = x < 0 ? 2 : 1;
		while ((x /= 10) != 0)
			++result;
		return result;
	}


	void intstr::assignValue(std::string input, invalid_input& exception) { assignValue(input, &exception); }

	void intstr::assignValue(std::string input, invalid_input * exception)
	{
		for (unsigned int i = 0; i < numData; ++i)
		{
			Data& dt = data[i];
			for (unsigned int i = 0; i < dt.numStrings; ++i)
				if (input == dt.string[i])
				{
					*var = dt.correspondsTo;
					return;
				}
		}

	 /* if (no string matches) */
		{
			string problem;
			string problemPostName = " was not provided a valid string.\n";
			for (unsigned int i = 0; i < numData; ++i)
			{
				for (int j = 0; j < lengthOfLongestCorrespondsToValue - lengthOfValue(data[i].correspondsTo); ++j)
					problemPostName += " ";

				problemPostName += to_string(data[i].correspondsTo) + ": ";

				for (unsigned int j = 0; j < data[i].numStrings; ++j)
					problemPostName += data[i].string[j];

				problemPostName += "\n";
			}

			if (!exception)
				throw runtime_error("A CIP::intstr " + problemPostName);

			if (exception->type == exception->flag)
				problem = "Flag '" + exception->name + "' argument " + to_string(exception->index + 1) + problemPostName;
			else
				problem = exception->name + problemPostName;
			
			exception->problem = problem;

			throw *exception;
		}
	}

	intstr::~intstr() { delete[] data; }
	intstr::Data::~Data() { delete[] string; }


	
}