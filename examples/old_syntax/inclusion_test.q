// Demonstrates object instances
include:
    Potato
end

main:
    // Instantiate two Potatoes, with their default position (x : 0, y : 0)
    new | Potato | potato_1 | (950, 450)
    new | Potato | potato_2 | (960, 450)

    // Print all values of the potatoes:
    potato_1.summary | ()
    potato_2.summary | ()

    new | Point | point_1 | (53, 68)
    new | Point | point_2 | (67, 43)

    point_1.summary | ()
    point_2.summary | ()
end
