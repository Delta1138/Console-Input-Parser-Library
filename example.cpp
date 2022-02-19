#include <iostream>
#include <string>
#include <CIP.h>
using namespace CIP;
using namespace std;

int main(int argc, char * argv[])
{
	// theoretically, variables used in the program should be defined out here, but I didn't do that so that it's easier to make a looping test of all this.
	{
		input_handler ih(argc, argv);

		int ov[4]; // ordered vars
		bool lastOrdered;
		bool Oind[2]; // indicators

		// otherVar is set by something else, say a flag in ih
		int otherVar = 0;

		CIP_LAMBDA(int, func) {
			if (argc != 2)
				throw exception();
			return *(int*)argv[0] + *(int*)argv[1];
		};

		ih.setMinOrdered(1);

		CIP_ORDERED_START(ih);
		ih.setOrdered(ov[0]);
		ih.setOrdered(ov[1], 100, Oind[0]);
		ih.setOrdered(ov[2], fref(func, ov[0], ov[1]));
		ih.setOrdered(ov[3], &otherVar, Oind[1]);
		ih.setOrdered(lastOrdered, true);
		CIP_ORDERED_END(ih);

		using namespace TYPE;

		bool Fi[3];


		int alpha;
		int beta;
		int gamma;
		CIP_LAMBDA(int, delta) {
			if (argc != 2)
				throw exception();
			return *(int*)argv[0] + *(int*)argv[1];
		};
		int epsilon;


		double zeta[3];

		double eta1;
		double eta2;

		double theta1;
		double theta2;
		double theta3;

		CIP_LAMBDA(double, kappa1) { return 1.1 + *(int*)argv[0]; };
		CIP_LAMBDA(double, kappa2) { return 2.2 + *(int*)argv[0]; };
		CIP_LAMBDA(double, kappa3) { return 3.3 + *(int*)argv[0]; };

		double iota[5];
		double x[6]; // set by something unrelated to inputHandler


		float vnd1; // vnd = void no default
		int vnd2;
		char vnd3;


		std::string someFilename = "string"; // set by something unrelated to inputHandler
		std::string v1; // v = void with default
		char v2;
		double v3;

#define elms(x) sizeof(x)/sizeof(x[0]) // number of elements in array x

		CIP_FLAGS_START(ih);
		ih.setFlag("no-args", Fi[0]);
		ih.setFlag("alpha", alpha);
		ih.setFlag("beta", beta, 10);
		ih.setFlag("gamma", gamma, fref(delta, ov[0], ov[1]), Fi[1]);
		ih.setFlag("epsilon", epsilon, &ov[2]);
		ih.setFlag("zeta", arrayVar(zeta), elms(zeta));
		ih.setFlag("eta", tvars(eta1, eta2), defaults(1.1, 2.2), 2);
		ih.setFlag("theta", tvars(theta1, theta2, theta3), defaults(fref(kappa1, ov[2]), fref(kappa2, ov[2]), fref(kappa3, ov[2])), 3);
		ih.setFlag("iota", arrayVar(iota), arrayVar(x), elms(iota));
		ih.setFlag("vnd", vars(vnd1, vnd2, vnd3), types(FLOAT, INT, CHAR), 3, Fi[2]);
		ih.setFlag("void", vars(v1, v2, v3), new void*[3]{ NO_FILE_EXTENSION(someFilename), new char{ 'x' }, new double*{&x[5]} }, types(FUNC::STR, CHAR, PTR::DOUBLE), 3);
		ih.setFlag("space");
		CIP_FLAGS_END(ih);

#undef elms

		std::string a = "a";
		std::string file = "1";

		
		CIP_TESTS_START(ih);
		ih.setTest(STRINGS_ARENT_EQUAL(a, file));
		ih.setTest(true, NOT, fileExistsAndCanBeOpened, file);
		ih.setTest(YESNO(std::cout, std::cin, NAME_PLUS(file, "The file '", "' will be overwriten! Are you sure? (y/n) "), false));
		CIP_TESTS_END(ih);
		


		ih.setupDynamicInput();
		try {
 			ih.interpretInput();
		}
		catch (invalid_input e)
		{
			cout << e.what() << '\n';
			// return 1;
		}
		catch (test_failed e)
		{
			cout << e.what() << '\n';
			// return 1;
		}
		catch (user_requested_help e)
		{
			return 2;
		}
		catch (excessive_input e)
		{
			cout << e.what() << '\n';
			// return 3;
		}
		catch (not_enough_inputs e)
		{
			cout << e.what() << '\n';
			// return 4;
		}
		catch (programming_error e)
		{
			cout << e.what() << '\n';
			// return 5;
		}

		ih.debug_listOutput();

		cout << "\n\n";
	}
    
	return 0;
}