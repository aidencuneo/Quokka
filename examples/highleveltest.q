fun sum *
    str = ''
    if type(argv[0]) == 'list'
        a = 0
        l = argv[0]
        length = len(l)
        while a < length
            str += l[a]
            a += 1
        end
    else
        a = 0
        while a < argc
            str += argv[a]
            a += 1
        end
    end

    ret str
end

s = sum('words', ' + ', 'more words')
println(s)
l = sum(['words', 'WORDS', 'worddd', 'WORDDD'])
println(l)
