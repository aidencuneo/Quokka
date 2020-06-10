fun pf 1
    obj = argv[0]
    s = '<' + type(obj) + '>[\n'
    i = 0
    while i < len(obj)
        s += '  ' + disp(obj[i]) + ',\n'
        i++
    end
    ret s + '](' + string(len(obj)) + ')'
end


lst = [
    53,
    23423,
    189,
    598,
    null,
    835,
]

println(pf(lst))

hey = 'hey'
lst += [hey.upper, (hey).upper, ((hey)).upper]

println(pf(lst))

// condition ? t : f

inc = lam 1 : argv[0] + 1
println(inc)
