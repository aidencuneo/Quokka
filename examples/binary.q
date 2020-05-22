fun joinany 2
    strlst = argv[0]
    joinstr = argv[1]
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
        b += [n - 2 * (n / 2)]
        n /= 2
        i += 1
    end
    ret joinany(b, '')
end

fun xor 2
    first = argv[0]
    secnd = argv[1]

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

    ret new
end

one = binary(234)
two = binary(17)
num = xor(one, two)
println(num)
