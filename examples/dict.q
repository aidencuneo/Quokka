names = ['aiden', 'someone else', 'another person']
values = [15, 16, 57]

fun get 1
    a = 0
    i = len(names)
    while a < i
        if names[a] == argv[0]
            ret values[a]
        end
        a += 1
    end
end

println(get('another person'))
println(get('someone'))
println(get('aiden'))
println(get('someone else'))
