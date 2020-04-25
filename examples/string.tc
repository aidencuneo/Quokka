// Demonstrates the creation of strings

main:
    // Declare integer named 'number' with value of 10:
    int     | number | 15

    // Assign the previous integer's value directly to
    // the value of a new string:
    string  | strnum | number

    // Create other strings to concatenate:
    string  | prefix | '[([([There are >\n\ngibberish'
    string  | suffix | '  p e o p l e   h e r e .'

    // Fix up the strings with slicing and reordering functions:
    set     | prefix | stringslice   | (prefix, 5, 12)
    set     | suffix | stringreorder | (suffix, 2)

    // Print the concatenated strings:
    println | stringcat | (prefix, strnum, suffix, "\0")
end
