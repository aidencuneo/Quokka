fun slice 3
    lst   = argv[0]
    start = argv[1]
    stop  = argv[2] == -1 ? len(lst) : argv[2]
    newthing = lst[0]

    i = 1
    while i < len(lst)
        newthing += lst[i]
        i++
    end

    ret newthing
end


fun rand
    num = 65536 ** 3
    ret int(slice(string(&num), 3, -1))
end


PRINTSEP = '\n'
println('Random number:', rand(), rand())
println << ['Random number:', rand(), rand()]
