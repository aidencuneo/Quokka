fun simprange 1
    lst = []; i = 0
    while i < argv[0]
        lst += [i]; i +
    end
    ret lst
end


// println(simprange(14))

integer = 80
lst = [80]
str = '[80]'

println(
    len(simprange.value),
    simprange.filepath,
    simprange.__call__,
)

println(
    integer.value,
    lst.value,
    str.value,
)

import 'builtins'

map(type, println, [integer, lst, str])
