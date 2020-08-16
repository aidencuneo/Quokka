// Capitalised functions are meant to symbolise classes
fun Card 2
    ret [
        '' + argv[0],
        '' + argv[1],
    ]
end

fun randint 2
    _min = int(argv[0])
    _max = int(argv[1])
    ret 0
    ret _min + (('' + (&&[]))[-1] * _max)
end

fun reset_deck
    deck = []

    i = 0
    while i < len(colours)
        deck += [Card(colours[i], 0)]
        j = 1
        until j > 9
            deck += [Card(colours[i], j)] + [Card(colours[i], j)]
            j++
        end

        deck += [Card(colours[i], '+2')] + [Card(colours[i], '+2')] // Plus two
        deck += [Card(colours[i], 'r')]  + [Card(colours[i], 'r')]  // Reverse
        deck += [Card(colours[i], 's')]  + [Card(colours[i], 's')]  // Skip

        i++
    end

    i = 0
    while i < 4
        deck += [Card('wild', 'w')]  // Wild card
        i++
    end

    i = 0
    while i < 4
        deck += [Card('wild', '+4')] // Plus four
        i++
    end

    println(len(deck))

    global deck
end

fun shuffle_deck
    println(7 - 8)

    i = len(deck) - 1
    while i >= 0
        other = randint(0, i)
        if other != i
            temp = deck[other]
            deck[i] = deck[other]
            deck[other] = temp
        end
        i--
    end

    // for index in range(len(container) - 1, 0, -1)
    //     other = random.randint(0, index)
    //     if other == index:
    //         continue
    //     container[index], container[other] = container[other], container[index]
    // end

    println(deck)
    global deck
end

fun start
    reset_deck()
    shuffle_deck()

    until!
        i = 0
        while i < len(players)
            println('-- Bot ' + i + ' has ' + len(players[i++]) + ' cards --')
        end

        println('-- Your hand (' + len(players[0]) + ' cards): --')
        k = input('> ').lower()
        k == 'exit' or k == 'quit' or k == 'q' ? exit()
    end
end

// A Player Object is a list of Card Objects
// First player (players[0]) is the user
players = [[]]
global players

deck = null
global deck

colours = [
    'red',
    'yellow',
    'green',
    'blue',
]
global colours

start()
