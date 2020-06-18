import * (
    'builtins',
)


fun binary 1
    n = argv[0]
    b = ''
    while n > 0
        b = string(n % 2) + b
        n /= 2
    end
    ret b
end


fun bintoint 1
    // Init
    num = argv[0]
    binary_val = num
    decimal_val = 0
    base = 1
    rem = 0

    while num > 0
        rem = num % 10
        decimal_val += (rem * base)
        num /= 10
        base *= 2
    end

    ret decimal_val
end


fun binxor 2
    both = lpad('0', [argv[0], argv[1]])
    first = both[0]
    secnd = both[1]

    // Perform XOR
    new = ''
    i = 0
    while i < len(first)
        if first[i] == secnd[i]
            new += '0'
        else
            new += '1'
        end
        i += 1
    end

    ret long(new)
end


print('Enter first number  : ')
one = 235 // long(input())

print('Enter second number : ')
two = 401 // long(input())

b_one = binary(one)
b_two = binary(two)

println('BINARY:')
println(b_one, '^', b_two, '\n')

num = binxor(b_one, b_two)
println(one, '^', two, '=', bintoint(num))
