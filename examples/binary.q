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

println(binary(234))
println(binary(17))
println(binary(68))
println(binary(45))
println(binary(35))
println(binary(79))
