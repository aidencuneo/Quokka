import * 'builtins'

fun random_digit
    item = '' + &[['[]', 257]]
    summed = sum(map(int, item))
    ret int(('' + summed)[-1])
end

lst = []
i = 0
while i < 1000
    lst += [random_digit()]
    i++
end

println(lst)

i = 0
while i < 10
    println(count(lst, i++))
end
