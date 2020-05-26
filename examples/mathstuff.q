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
    ret 'stringggg'
end

inum = oof()
println(inum)

lnum = oop()
println(lnum)

str = oos()
println(str + string(lnum))

println(type(inum), type(lnum), type(str))

if type(inum) == 'int'
    println("Yup, inum is an int")
end
