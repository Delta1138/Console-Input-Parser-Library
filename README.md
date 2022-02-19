# Console Input Parser Library
 
The documentation provided with this library uses pages from cppreference.com as templates. As such, that specific part of this repository is licensed under the same licenses as cppreference.com (Creative Commons Attribution-Sharealike 3.0 Unported License (CC-BY-SA) and by the GNU Free Documentation License (GFDL) (unversioned, with no invariant sections, front-cover texts, or back-cover texts).

(cppreference is a wiki which aims to be an accessible version of the C++ standard)

they provide the licenses which they use here:
https://en.cppreference.com/w/Cppreference:Copyright/GDFL
https://en.cppreference.com/w/Cppreference:Copyright/CC-BY-SA


Otherwise, you can do anything you like with this, and I am not liable for anything related to this (code provided as-is and all that).


Legal preamble out of the way, documentation can be found in the marked folder. "home.html" in that folder is the place to start, I reccommend creating a shortcut to it which is accessable outside the folder somewhere.
I got a bit lazy with documentation near the end of this. As a result, I did not optimise the full example provided with the library to demonstrate the most things possible, and there are a few things which are not currently in the documentation.
The latter is very likely to change whenever I update this. The former may not, unless someone makes a good example for me.
This library has been tested to a reasonable extent, but there may still be significant glitches. There are certainly some flaws with how it works, all of which are documented. If said flaws get in your way, or there are glitches, tell me and I'll fix whatever is giving you problems.





Things that aren't yet properly documented:


- There is a documented function ternary, which takes a bool and two c-strings and returns a c-string based on the bool. There is also a function 'choose', which acts like a switch statement. Its first param is an int, the rest are c-strings (variadic function). It returns the string corresponding to the int, where 0 --> the first string.
ternary has variants Fternary, ternaryF, FternaryF. choose has the same variants, which do the same things.

- there is a documented function vars which returns a void* array. There is also a function 'tvars', which works the same way but returns a type* array, where type is the type of all its vars (all its inputs must be the same type, they are taken by reference, function is variadic).

- there is a function 'defaults' which takes values of the same type and returns an array containing those values. It's meant to be used in a similar way to vars, just for defaultVals arguments rather than vars arguments. It can accept all kinds of defaults (constant values, CIP::functions, pointer-to-values) properly. It does not take its arguments by reference, since it has to be able to handle constant values (including literals).


- default values for boolstr variables must be bools, which will be directly applied to the bool contained in the bool str. Similarily, default values for intstrs must be ints.


- Calling overloads of input_handler member functions which are more powerful than what you need is generally not a good idea, as they were not programmed with the possibility a programmer would do so in mind. 
 Specific rules: Function template overloads of setFlag() and setOrdered() should not be called with defaultVal(s) == nullptr. The 'pure' overload of setFlag(), which is 
setFlag( string name, void* vars[], void* defaultVals[], type types[] int argc, bool* indicator = nullptr );
should not be called if every defaultVal is of the same type. 
 If either of those rules is broken, undefined behavior will be invoked at some point, most likely during interpretInput() or during input_handler's destructor respectively.


- if an input to a bool can be interpreted as an integer which is equal to zero (e.g.s 0, 0000, -0, -00, but not 0.0), that bool will be set to false. Otherwise, it will be set to true.


- Ints accept floating point inputs, which are (iirc) rounded towards zero. They currently just pass the input string to stoll and stoull without much else happening (overflow and sign checking is present, otherwise there isn't anything).


- Lastly, input_handler breaks/behaves strangely if interpretInput() is called more than once