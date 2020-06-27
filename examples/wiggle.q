import 'builtins'


fun range 2
    lst = []
    i = argv[0]
    while i < argv[1]
        lst += [i]
        i +
    end
    ret lst
end


fun re_range 2
    lst = []
    i = argv[1]
    while i > argv[0]
        lst += [i]
        i -
    end
    ret lst
end


fun create 1
    ret ' ' * argv[0] + 'wiggle'
end


lst = map(create, range(0, 10) + re_range(1, 9))

while true
    i = 0
    while i < len(lst)
        println(lst[i])
        i +
        j = 0
        while j < 1000
           j +
        end
    end
end
