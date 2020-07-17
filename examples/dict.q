names = ['aiden', 'someone else', 'another person']
values = [15, 16, 57]

fun get 3
    n = argv[0]
    v = argv[1]

    a = -1
    while ++a < len(n)
        if n[a] == argv[2]
            ret v[a]
        end
    end
end

println(get(names, values, 'another person'))
println(get(names, values, 'someone'))
println(get(names, values, 'aiden'))
println(get(names, values, 'someone else'))
