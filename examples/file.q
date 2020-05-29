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


fun max 1
    lst = argv[0]
    lstlen = len(lst)
    largest = 0

    i = 0
    while i < lstlen
        if lst[i] > largest
            largest = lst[i]
        end
        i +
    end

    ret largest
end


fun lpad 2
    pad = argv[0][0] // Limit pad to one char
    lst = argv[1]
    new = []

    lstlen  = len(lst)
    largest = max(map(len, lst))

    i = 0
    while i < lstlen
        new += [pad * (largest - len(lst[i])) + lst[i]]
        i +
    end

    ret new
end


fun map 1 *
    lst = argv[-1]
    new = []

    lstlen = len(lst)
    i = 0

    while i < lstlen
        item = lst[i]

        j = argc - 2
        while j >= 0
            item = argv[j](item)
            j -
        end

        new += [item]
        i +
    end

    ret new
end


fun same 1
    lst    = argv[0]
    item   = lst[0]
    lstlen = len(lst)

    i = 0
    while i < lstlen
        if lst[i] != item
            ret false
        end
        i +
    end

    ret true
end


fun stat 2
    char = argv[0]
    lst  = argv[1]

    padded = lpad(' ', lst)

    lens = map(int, println, padded)
    println(lens)

    ret same(lens)
end


if stat(' ', ['words', 'nekooo', 'inu', 'yup', 'yesssssss', 'nooo'])
    println('Worked.')
else
    println('Hmm')
end
