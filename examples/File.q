import 'lib/Quokka'

def string construct_func(string name, string args)
    args = args.slice(1, 1)
    println name
    println args
end

main:
    string function = construct_func('bob', '(50, 60, 70)')
end
