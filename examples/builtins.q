fun min 1
    lst = argv[0]
    lstlen = len(lst)
    smallest = lst[0] + 1

    i = 0
    while i < lstlen
        if lst[i] < smallest
            smallest = lst[i]
        end
        i +
    end

    ret smallest
end


fun max 1
    lst = argv[0]
    lstlen = len(lst)
    largest = lst[0] - 1

    i = 0
    while i < lstlen
        if lst[i] > largest
            largest = lst[i]
        end
        i +
    end

    ret largest
end


// Left Pad (Align list items to left)
fun lpad 1 2
    if argc == 1
        pad = ' '
        lst = argv[0]
    else
        pad = argv[0][0] // Limit pad to one char
        lst = argv[1]
    end

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


// Centre Pad (Align list items to centre)
fun cpad 1 2
    if argc == 1
        pad = ' '
        lst = argv[0]
    else
        pad = argv[0][0] // Limit pad to one char
        lst = argv[1]
    end

    new = []

    lstlen  = len(lst)
    largest = max(map(len, lst))

    i = 0
    while i < lstlen
        most = pad * ((largest - len(lst[i])) / 2) + lst[i]
        new += [most + (pad * (largest - len(most)))]
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

        j = 0
        m = argc - 1
        while j < m
            item = argv[j](item)
            j +
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


fun any 1
    lst    = argv[0]
    lstlen = len(lst)

    i = 0
    while i < lstlen
        if lst[i]
            ret true
        end
        i +
    end

    ret false
end


fun all 1
    lst    = argv[0]
    lstlen = len(lst)

    i = 0
    while i < lstlen
        if not lst[i]
            ret false
        end
        i +
    end

    ret true
end


fun split 1 2
    if argc == 1
        at  = ' '
        str = argv[0]
    else
        at  = argv[0]
        str = argv[1]
    end

    strlen  = len(str)
    current = ''
    lst     = []

    i = 0
    while i < strlen
        if str[i] == at
            lst += [current]
            current = ''
        else
            current += str[i]
        end
        i +
    end

    ret lst + [current]
end


fun strsum *
    str = ''

    if type(argv[0]) == 'list'
        a = 0
        l = argv[0]
        length = len(l)
        while a < length
            str += l[a]
            a += 1
        end
    else
        a = 0
        while a < argc
            str += argv[a]
            a += 1
        end
    end

    ret str
end
