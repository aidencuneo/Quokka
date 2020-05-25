fun modulo 2
    ret argv[0] - argv[1] * (argv[0] / argv[1])
end

fun lpadwith 3
    pad = argv[0]
    pad = pad[0] // Limit pad to one char

    first = argv[1]
    secnd = argv[2]

    // Pad until both strings are the same length
    if len(secnd) > len(first)
        i = 0
        while i < (len(secnd) - len(first)) + 1
            first = pad + first
            i += 1
        end
    elif len(first) > len(secnd)
        i = 0
        while i < (len(first) - len(secnd)) + 1
            secnd = pad + secnd
            i += 1
        end
    end

    ret [first, secnd]
end

// fun flip 1
//     str = argv[0]
//     out = ''

//     i = len(str)
//     while i >= 0
//         out += str[+i]
//         i -= 1
//     end

//     ret out
// end

fun binary 1
    n = argv[0]
    b = ''
    i = 0
    while n > 0
        b = string(modulo(n, 2)) + b
        n /= 2
        i += 1
    end
    ret b
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

    both = lpadwith('0', first, secnd)
    first = both[0]
    secnd = both[1]

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

print('Enter first number  : ')
one = 9999//long(input())

print('Enter second number : ')
two = 7777//long(input())

println('BINARY:')
println(binary(one), '^', binary(two), '\n')

num = xor(binary(one), binary(two))
println(one, '^', two, '=', bintoint(num))
