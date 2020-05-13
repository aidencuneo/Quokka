nulls = []
a = 0

while a < 10000
    nulls += [null]
    a += 1
end

println(nulls)
println(nulls[-1])
println(string(100))
println(null)

// Method 1:
// lst += [5]

// This method of appending items to lists expands into:
// lst = lst + [5]
// Which creates a new temporary list to store `lst` and `[5]` before
// combining them.
// This method results in extra memory being used, but it's a quick way to
// print out two lists combined into one.


// Method 2:
// lst.add([5])

// This method has not been implemented yet.
