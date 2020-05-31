names = ['aiden', 'someone else', 'another person']
values = [15, 16, 57]

fun get 3
    d = argv[0]
    v = argv[1]

    a = 0
    i = len(d)

    while a < i
        if d[a] == argv[2]
            ret v[a]
        end
        a +
    end
end

println(get(names, values, 'another person'))
println(get(names, values, 'someone'))
println(get(names, values, 'aiden'))
println(get(names, values, 'someone else'))
