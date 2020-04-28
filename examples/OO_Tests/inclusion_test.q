// Demonstrates object instances
include:
    Potato
end

nice1:
    ret | 69
end

def | int | nice2 | int num
    ret | num + 69
end

main:
    // Instantiate two Potatoes, with their default position (x : 0, y : 0)
    new | Potato | potato_1 | (0, 0)
    new | Potato | potato_2 | (0, 0)

    // Print all values of the potatoes:
    print   | 'Potato 1: '
    print   | potato_1.x
    print   | ','
    println | potato_1.y

    print   | 'Potato 2: '
    print   | potato_2.x
    print   | ','
    println | potato_2.y
end
