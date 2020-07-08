// INT_MAX  = +(2**32L/2)
// UINT_MAX = 2**32L
// LONG_MAX = 2**62L*2-1


// Right Pad (Align list items to right)
fun rpad 1 2
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
        i++
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
        i++
    end

    ret new
end


// Pad num (Pad list items to be the length of a given number)
fun padnum 3
    pad = argv[0][0] // Limit pad to one char
    num = argv[1]
    lst = argv[2]

    new = []

    lstlen  = len(lst)

    i = 0
    while i < lstlen
        new += [pad * (num - len(lst[i])) + lst[i]]
        i++
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
        while j < argc - 1
            item = argv[j](item)
            j++
        end

        new += [item]
        i++
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
        i++
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
        i++
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
        i++
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
        i++
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
            a++
        end
    else
        a = 0
        while a < argc
            str += argv[a]
            a++
        end
    end

    ret str
end


fun pf 1
    obj = argv[0]
    s = '<' + type(obj) + '>[\n'
    i = 0
    while i < len(obj)
        s += '  ' + disp(obj[i]) + ',\n'
        i++
    end
    ret s + '](' + string(len(obj)) + ')'
end
