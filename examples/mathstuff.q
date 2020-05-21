integer = 2147483647
longnum = 2147483648

println(integer, ':', type(integer))
println(longnum, ':', type(longnum))

bignum = integer * longnum
println(bignum, ':', type(bignum))

println(4 ** 4)
println((4 + 4) - ((4 * 4) / (4 ** 4)))
