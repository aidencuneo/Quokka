fun modulo 2
    ret argv[0] - argv[1] * (argv[0] / argv[1])
end

fun joinany 2
    joinstr = argv[0]
    strlst = argv[1]
    outstr = ''

    i = 0
    while i < len(strlst)
        outstr += string(strlst[i]) + joinstr
        i += 1
    end

    ret outstr
end

fun binary 1
    n = argv[0]
    b = []
    i = 0
    while n > 0
        b += [modulo(n, 2)]
        n /= 2
        i += 1
    end
    ret long(joinany('', b))
end

fun bintoint 1
    // Init
    num = argv[0]
    binary_val = num
    decimal_val = 0
    base = 1
    rem = 0

    while num > 0
        rem = modulo(num, 10)
        decimal_val += rem * base
        num /= 10
        base *= 2
    end

    ret decimal_val
end

fun xor 2
    first = string(argv[0])
    secnd = string(argv[1])

    // Pad the numbers
    if len(secnd) > len(first)
        i = 0
        while i <= (len(secnd) - len(first)) + 1
            first = '0' + first
            i += 1
        end
    elif len(first) > len(secnd)
        i = 0
        while i <= (len(first) - len(secnd)) + 1
            secnd = '0' + secnd
            i += 1
        end
    end

    // Perform XOR
    new = ''
    i = 0
    while i < len(first)
        if first[i] == secnd[i]
            new += '0'
        else
            new += '1'
        end
        i += 1
    end

    ret long(new)
end

// print('Enter first number  : ')
// one = long(input())

// print('Enter second number : ')
// two = long(input())

// if one > 1048574
//     one = 1048574
// end

// if two > 1048574
//     two = 1048574
// end

// println('\nBINARY:')
// println(binary(one), '^', binary(two), '\n')

println(sizeof(''), sizeof('aiden'))
println(sizeof(0), sizeof(2398434))
println(sizeof(0L), sizeof(999999999999999L))

lst = ['aiden', 'bob', 'ckjsndjkd', 13, 6734, 239384L, 2398L, 98343729847394, null, 3489]
println(sizeof(lst))
println((sizeof(lst) - 32) / 4)
println(len(lst))

// num = xor(binary(one), binary(two))
// println(one, '^', two, '=', bintoint(num))
