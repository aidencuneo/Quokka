import 'lib/Quokka'

def string construct(string name, string args)
    args = args.slice(1, 1)
    println name + ', ' + args
end

main:
    string function = construct('bob', '(50, 60, 70)')
end
