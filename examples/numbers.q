fun title
    print('|' + ('-' * 5) + 'INT'  + ('-' * 5))
    print('|' + ('-' * 9) + 'LONG' + ('-' * 9))
    print('|\n')
end

num = 1L
count = 60

title()

a = 0
until a is count
    num *= 2

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
