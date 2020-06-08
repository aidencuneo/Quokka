time = 0
bag = []


fun main
    i = input(display_time() + '> ')

    if i == 'bag'
        check_bag()
    elif i == 'exit'
        println('Goodbye.')
        exit()
    else
        println("Sorry, I don't understand.")
    end

    time++
end


fun check_bag
    println(bag)
end


fun display_time
    t = padnum(
        '0', 2,
        [
            string(time / 60),
            string(modulo(time, 60))
        ]
    )

    ret t[0] + ':' + t[1]
end
