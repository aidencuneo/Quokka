// oof() - Returns type int
fun oof
    ret 2147483647
end

// oop() - Returns type long
fun oop
    ret 2147483648
end

// oos() - Returns type string
fun oos
    ret '2147483647'
end

num = oof()
println(num)

num = oop()
println(num)

str = oos()
println(str + string(num) + str)
