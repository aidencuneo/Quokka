fun lpadwith 3
    pad = argv[0]
    pad = pad[0] // Limit pad to one char

    first = argv[1]
    secnd = argv[2]

    // Pad until both strings are the same length
    if len(secnd) > len(first)
        i = len(first)
        while i < len(secnd)
            first = pad + first
            i +
        end
    elif len(first) > len(secnd)
        i = len(secnd)
        while i < len(first)
            secnd = pad + secnd
            i +
        end
    end

    ret [first, secnd]
end


fun map 2
    func = argv[0]
    lst = argv[1]
    new = []

    i = 0
    lstlen = len(lst)
    while i < lstlen
        new += [func(lst[i])]
        i +
    end

    ret new
end


padded = lpadwith(' ', "I've almost written them all down.", "There's a heap of joints")
map(println, padded)
