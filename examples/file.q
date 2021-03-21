import * 'builtins'

lst = ['hello', 'one hundred', 'two hundred', 'thirty']
cpad_lst = cpad(lst)
println(lst)
println(cpad_lst)

map(println, cpad_lst)
