// Demonstrates string replacement
include:
    potato.h

main:
    // Instantiate two Potatoes, with their default position (x : 0, y : 0)
    object | potato_1 | potato
    object | potato_2 | potato

    // Create and print the original string:
    println | potato_1 | x | y
    println | potato_2 | x | y

    // Print new string
    println | nstr
end
