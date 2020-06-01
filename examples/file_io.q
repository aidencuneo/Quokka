//
// -- WRITING --
//

// Open the file for writing
f = fopen('file.txt', 'w')

// Write to the file
f + 'Hello there, this is a file.\n'

// Close the file
- f


//
// -- READING --
//

// Open the file for reading
f = fopen('file.txt')

// Print the contents of the file
println(f)

// Close the file
- f


//
// -- CREATING A NEW FILE --
//

// This will create a new file, then immediately close it
- fopen('file.txt', 'w')
