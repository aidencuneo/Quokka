// Demonstrates object instances
include:
    nonexistentheader.q
    potato.q
    thing.q
end

main:
    // Instantiate two Potatoes, with their default position (x : 0, y : 0)
    object | potato | potato_1
    object | potato | potato_2

    // Print all values of the potatoes:
    print   | 'Potato 1 '
    print   | potato_1.x
    print   | ', '
    println | potato_1.y

    print   | 'Potato 2 '
    print   | potato_2.x
    print   | ', '
    println | potato_2.y
end
