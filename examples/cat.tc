// Demonstrates string concatenation

main:
    // Create four strings which will be concatenated:
    string | first  | 'Hello'
    string | second | ','
    string | third  | ' World'
    string | fourth | '!'

    // Concatenate them into one new string. Note that the stringcat
    // function requires a single \0 string as it's final argument to
    // successfully end the string and avoid a segmentation fault.
    string  | cat | stringcat | (first, second, third, fourth, "\0")
    println | cat
end
