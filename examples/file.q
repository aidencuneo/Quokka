import 'builtins.q'


fun simprange 1
    lst   = []
    up_to = argv[0]

    i = 0
    while i < up_to
        lst += [i]
        i +
    end

    ret lst
end


i = 10
j = ''
while not len(j) or i
    i -
    j += '.'
    println(i, j)
end


fun wrap 1
    ret '| ' + argv[0] + ' |'
end


len = max(map(wrap, println, cpad([
    'text',
    'give me more text',
    'this must be aligned perfectly'
])))

println('-' * (+ len - 1))
