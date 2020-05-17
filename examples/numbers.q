fun double
    num *= 2
end

fun title
    print('|' + '-' * 4 + 'INT'  + '-' * 5)
    print('|' + '-' * 8 + 'LONG' + '-' * 9)
    print('|\n')
end

num = 1L
count = 60

title()

a = 0
until a == count
    double()

    lpadding = ' ' * int(11 - len(string(+num)))
    rpadding = ' ' * int(20 - len(string(num)))

    print('|')
    print(lpadding, +num, '|')
    print(rpadding, num, '|\n')

    a += 1
end

title()

print('\n')
println('Final (long) :', num)
println('Final (int)  :', +num)
