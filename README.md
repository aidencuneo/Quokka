# Quokka

An Object Oriented programming language with a neat, complexity-free syntax.
Quokka compiles code into a simple bytecode format which is then interpreted for extra fast code interpretation, with a tested code execution speed of over 4 times faster than Python 3.8.

Quokka currently features 5 main built-in datatypes (it will feature many more in the future), integer (int), long, string, list, and null, which can be created with constants or their respective functions.

In the future, integers and longs will be combined into one `int` datatype, which will be able to hold numbers with no arbitrary length. But for now and the near future, we have the two datatypes: `int` and `long`.

## Datatypes

### Integer

Integers are standard numbers which generally have a limit of up to 10 digit numbers (highest int is exactly 2147483647 for machines with 4 byte integers)

An integer can be created using the `int` function, or simply by typing a number: `123`.

### Long

Longs are double length numbers which generally have a limit of up to 19-20 digit numbers (highest long is exactly 9223372036854775807 for machines with 8 byte longs)

### String

A string is simply a string of letters. A string can be created by placing any list of letters into single or double quotes, for example: `'This is an example of a string.'`.

Another way to create strings is by using the `string` function using another datatype as it's argument, which will convert that datatype into a string.
An example of this would be using the `string` function on an integer to print the integer combined with some text: `println('There are ' + string(10) + ' Quokkas.')`

### List

A list in Quokka is an array of items of any datatype, with no set length. Lists are dynamic, mutable, and they can hold other lists within them.

To create a list, surround a list of items with `[` and `]`, like this: `[1, 2, '3', 'word']`.

To retrieve an item from a list, use the index operator: `[0]` or `[n]` (indexes start from 0 for lists).

### Null

Null is a placeholder type in Quokka, and it is returned when an index operator could not retrieve an item due to the index being out of range, and it is also returned from any functions that are missing a `ret` statement, or any form of return value.
