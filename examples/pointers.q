// Declaring two variables with the EXACT same constant

num_one = 12
num_two = num_one

println(string(num_one) + ', ' + string(num_two))

println('num_one == num_two:', num_one == num_two)
println('num_one is num_two:', num_one is num_two)
println('')

// Declaring two variables using separate constants

num_one = 15
num_two = 15

println(string(num_one) + ', ' + string(num_two))

println('num_one == num_two:', num_one == num_two)
println('num_one is num_two:', num_one is num_two)
println('')

// Using the address operator

str_var = 'This is a string'
println('Address of str_var is', &str_var)

println(println)
