# Quokka

An Object Oriented programming language with a neat, complexity-free syntax.
Quokka compiles code directly into C for extra fast code execution, with a tested execution speed of approximately 4 times faster than Python 3.8.

Quokka currently features 3 built-in datatypes (it will feature many more in the future):
integer, string, and bool, which can be instantiated with primitives such as 53, 'some words', or true (respectively), or with their class constructors.

To create a new object using a class constructor, simply state the object type, the name for the variable to store the object in, and call the class constructor with `new classname` or `new classname(argument1, argument2, etc)`.
For example:

    MyClass varname = new MyClass
