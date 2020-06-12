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


condition = 15

thing1 = len(string(condition)) == 2 ? true + 1
thing2 = len(string(condition)) == 2 ? true + 1 : false - 1

println(thing1, thing2)
