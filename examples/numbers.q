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

println(': Modulo')
println(23948239842739844L % 10)
println(12 % 5)
println('')

println(': XOR')
println(73 ^ 134)
println(73 ^ 134 ^ 73)
println('')

println(': Bitwise left-shift')
println(42 << 1)
println(50 << 3)
println('')

println(': Bitwise right-shift')
println(42 >> 1)
println(50 >> 3)
println('')
