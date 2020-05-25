fun binary 1
    n = argv[0]
    b = ''
    while n > 0
        b = string(n - 2 * (n / 2)) + b
        n /= 2
    end
    ret b
end

println(1180)
println(binary(1180))

i = 0
while i < 64 ** 2
    println(binary(i))
    i += 1
end
