lst1 = [2, 4, 6]
len1 = lst1.length
len2 = [2, 3].length

println(lst1, len1, len2)

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
