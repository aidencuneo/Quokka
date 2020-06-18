import 'builtins'

//
// UTILITIES
//


fun binary 1
    n = argv[0]
    b = []

    while n > 0
        b = [n % 2] + b
        n /= 2
    end

    while len(b) % 8
        b = [0] + b
    end

    ret b
end


fun binary_s 2
    n    = argv[0]
    size = argv[1]

    b = []

    while n > 0
        b = [n % 2] + b
        n /= 2
    end

    while len(b) < size
        b = [0] + b
    end

    ret b
end


fun bintoint 1
    binlst = argv[0]

    val  = 0
    base = 1
    rem  = 0

    i = len(binlst) - 1
    while i >= 0
        val += (binlst[i] * base)
        base *= 2
        i -
    end

    ret val
end


fun wait 1
    i = 0
    while i < argv[0]
        i +
    end
end


//
// Memory Stuff
//


fun create_memory 1
    size = argv[0]
    lst  = []

    i = 0
    while i < size * 8
        lst += [0]
        i +
    end

    ret lst
end


fun show_bytes
    i = 0
    while i < len(mem)
        print(mem[i])
        i +
    end
    print('\n')
end


// Return a pointer to the next free memory location
// (if a free memory location can be found)
fun find_free_memory
    i = 0
    while i < len(mem)
        if (not mem[i]     and
            not mem[i + 1] and
            not mem[i + 2] and
            not mem[i + 3] and
            not mem[i + 4] and
            not mem[i + 5] and
            not mem[i + 6] and
            not mem[i + 7]
        )
            ret i / 8
        end
        i += 8
    end

    ret -1
end


fun memclear 1
    location = argv[0][0]
    size     = argv[0][1]

    i = 0
    while i < size * 8
        mem[(location * 8) + i] = 1
        i +
    end
end


fun free 1
    location = argv[0][0]
    size     = argv[0][1]

    i = 0
    while i < size * 8
        mem[(location * 8) + i] = 0
        i +
    end
end



fun memset 2
    location = argv[0][0]
    size     = argv[0][1]
    value    = argv[1]

    // Make sure to cap the value to fit into it's allocated space
    // limit = 2L ** (size * 8) - 1
    // if value > limit
    //     value = limit
    // end

    bin_val = binary_s(value, size * 8)

    i = 0
    while i < size * 8
        if bin_val[i]
            mem[(location * 8) + i] = 1
        else
            mem[(location * 8) + i] = 0
        end

        i +
    end
end


fun malloc 1
    pointer_size = argv[0]

    // Set allocated space to 0
    free_memory = find_free_memory()
    if free_memory is -1
        ret [-1, 0]
    end

    memclear([free_memory, pointer_size])

    ret [free_memory, pointer_size]
end


fun mem_binary_get 1
    pointer = argv[0]

    lst = []

    i = pointer[0] * 8
    while i < pointer[0] + pointer[1] * 8
        lst += [mem[i]]
        i +
    end

    ret lst
end


fun memget 1
    ret bintoint(mem_binary_get(argv[0]))
end


fun strcpy 2
    pointer = argv[0]
    chars   = map(charcode, binary, argv[1])

    ind = pointer[0] * 8
    c = 0
    while c < len(chars)

        i = 0
        while i < len(chars[c])
            mem[ind + i] = chars[c][i]
            i +
        end

        if c > pointer[1]
            println('Segmentation Fault')
            exit()
        end

        ind += 8
        c +
    end
end


fun printf 1
    pointer = argv[0]

    c = pointer[0]
    while c <= pointer[1]
        print(tochar(memget([c, 1])))
        c +
    end
end


mem = create_memory(64) // Create 64 bytes of memory
global mem

// Allocate 1 byte and set it to 100
char1 = malloc(1)
memset(char1, 100)

// Allocate 1 byte and set it to 23
char2 = malloc(1)
memset(char2, 23)

// Allocate 16 bytes and copy a string into them
chars = malloc(32)
strcpy(chars, 'This is a pretty cool sentence..')

// Allocate a pointer and set it to 200
ptr = malloc(1)
memset(ptr, 200)

// Print the pointer's value
println(memget(ptr)) // 200

// Free the pointer
free(ptr)

// Print the pointer's value again
println(memget(ptr)) // 0

printf(chars)
print('\n')

println(memget(char1))
println(memget(char2))

show_bytes()
