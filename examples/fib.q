fun fibinc 1
    lst = argv[0]
    ret [lst[-1], lst[0] + lst[-1]]
end

l = [0L, 1L]

a = 0
while a < 90
    l = fibinc(l)
    println(l[-1])
    a +
end

// :   0.073s
// :   0.062s
// :   0.058s
// AVG 0.061s
