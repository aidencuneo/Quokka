// Demonstrates object instances
include:
    potato.h

main:
    // Instantiate two Potatoes, with their default position (x : 0, y : 0)
    object | potato_1 | potato
    object | potato_2 | potato

    // Print all values of the potatoes:
    println | potato_1 | 'Potato 1' | x | ',' | y
    println | potato_2 | 'Potato 2' | x | ',' | y
end
