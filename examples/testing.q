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

fun flip 1
    str = argv[0]
    out = ''

    i = len(str)
    while i >= 0
        out += str[+i]
        i -= 1
    end

    ret out
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
    ret flip(joinany('', b))
end

println(23)
println(binary(23))
