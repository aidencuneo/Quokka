# Quokka

An Object Oriented programming language with a neat, complexity-free syntax.
Quokka compiles code into a simple bytecode format which is then interpreted for extra fast code interpretation, with a tested code execution speed of over 4 times faster than Python 3.8.

Quokka currently features a few main built-in datatypes (it will feature many more in the future), `int`, `string`, `list`, and `null`, which can be created with constants or their respective functions. Other datatypes also exist, including `file`, `module`, `cmethod`, `cfunction` etc, although these datatypes are mainly created through built-in functions in Quokka, rather than by literals or constants.

~~In the future, integers and longs will be combined into one `int` datatype, which will be able to hold numbers with no arbitrary length. But for now and the near future, we have the two datatypes: `int` and `long`.~~

The `int` datatype can now hold numbers with no arbitrary length. Arithmetic operations on the `int` datatype are currently in an experimental stage, but improvements will constantly be made until the `int` datatype is able to be used at a much more suitable level.


# Features that are on their way

## Improved Arithmetic Operations

Arithmetic between integers will soon be majorly improved, and we're hoping to be able to perform mathematical operations on all types of natural numbers with 100% accuracy in the near future.

## Breadcrumbs/Traceback system

Currently, 

## User-defined Class Creation


# Datatypes

### Int

Integers (in Quokka) are standard natural numbers with no decimal or floating point and no arbitrary length.

An integer can be created using the `int` function, or by typing a numeric literal, such as `256`.

Every `int` object owns a base, which by default is 10. If an `int` is created with a numeric literal beginning with or equal to `0x` (for hexadecimal), then the `int` object created will own a base of 16, which will be used when that `int` object is displayed.

Performing mathematical operations between two `int`'s of different bases will result in the output `int` being the same base as the first `int` in the expression.

Knowing that `int` objects each have their own base, you can easily convert numbers to and from other bases in one of the methods shown below:

#### Decimal to Hexadecimal

    0x + 15
    --> 0xF

    0x + 231
    --> 0xE7

#### Hexadecimal to Decimal

    0 + 0xF
    --> 15

    0 + 0x5A
    --> 90


### String

A string is simply a string of letters. A string can be created by placing any list of letters into single or double quotes, for example: `'This is an example of a string.'`.

Strings automatically convert objects that are added to them into strings, so `'Age: ' + 15` will return `'Age: 15'`, which means using the `string` function isn't required, although the `string` function still exists for any reason that you may need a function to do that for you.

### List

A list in Quokka is an array of items of any datatype, with no set length. Lists are dynamic, mutable, and they can hold other lists within them.

To create a list, surround a list of items with `[` and `]`, like this: `[1, 2, '3', 'word']`.

To retrieve an item from a list, use the index operator: `[0]` or `[n]` (indexes start from 0).

### Null

Null is a placeholder type in Quokka, and it is returned when an index operator could not retrieve an item due to the index being out of range, and it is also returned from any functions that are missing a `ret` statement or any form of return value.
