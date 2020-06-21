// Open and print the contents of a given file, with no extra newline.
// Print nothing if the given file does not exist or can not be read.

// Python version (59 chars):
// try:print(open(input()).read(),end='')
// except:print(end='')

// Quokka version 1 (36 chars):
// f=open(input()).read()
// print(f?f:'')

// Quokka version 2 (34 chars):
// open(input()).read()
// print(_?_:'')

// Quokka version 3 (31 chars):
// open(input())
// _?print(_.read())
