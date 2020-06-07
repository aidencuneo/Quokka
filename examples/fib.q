fun fib 1
    l = [0L, 1L]
    res = []

    a = 0
    while a < argv[0]
        l = [l[-1], l[0] + l[-1]]
        res += [l[-1]]
        a++
    end

    ret res
end

// :   0.073s
// :   0.062s
// :   0.058s
// AVG 0.061s
