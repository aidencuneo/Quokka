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


p = pf([1, 2, 3])

input('[y/n]').lower() == 'y' ? println(p)
println('heyo'.lower().upper().lower())
