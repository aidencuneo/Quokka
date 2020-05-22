names = ['aiden', 'emma', 'someone else']
values = [15, 16, 57]

arg1 = ''
return = 0

fun get
    return = ''

    a = 0
    i = len(names)
    while a < i
        if names[a] == arg1
            return = values[a]
            a == i
        end
        a += 1
    end
end

arg1 = 'emma'
get()
println(return)

arg1 = 'someone'
get()
println(return)

arg1 = 'aiden'
get()
println(return)

arg1 = 'someone else'
get()
println(return)

