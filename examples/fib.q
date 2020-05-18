fun fibinc
    lasttwo = [lasttwo[-1], lasttwo[0] + lasttwo[-1]]
    ret lasttwo[-1]
end

lasttwo = [0L, 1L]

a = 0
while a < 90
    println(fibinc())
    a += 1
end
