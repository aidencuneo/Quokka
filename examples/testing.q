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
    b = ''
    i = 0
    while n > 0
        b += string(n - 2 * (n / 2))
        n /= 2
        i += 1
    end
    ret flip(b)
end

println(1180)
println(binary(1180))

i = 0
while i < 32
    println(binary(i))
    i += 1
end
