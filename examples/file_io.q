//
// -- WRITING --
//

// Open the file for writing
f = open('file.txt', 'w')

// Write to the file
f + 'Hello there, this is a file.\n'

// Close the file
- f


//
// -- READING --
//

// Open the file for reading
f = open('file.txt')

// Print the contents of the file
println(f.read())

// Close the file
- f


//
// -- CREATING A NEW FILE --
//

// This will create a new file, then immediately close it
- open('file.txt', 'w')
