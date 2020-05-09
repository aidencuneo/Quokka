// Operators:
// +------------------------+
// | +  | -  | *  | /  | =  |
// +------------------------+
// | <  | >  | <= | >= | == |
// +------------------------+
// | += | -= | *= | /= |    |
// +------------------------+

// Functions (in order of most to least complete):
// print
// println
// input
// string
// int (not complete)

i = input()

a = 0
while a < 50
    print(i[a])
    a +=  ++1  +  +++1
end

print('\n')

// LOAD_NAME a
// LOAD_INT 1
// LOAD_INT 1
// BINARY_ADD
// BINARY_ADD
// STORE_NAME a


// a
// 1
// 1
// UNARY_ADD
// BINARY_ADD
// UNARY_ADD
// BINARY_ADD
// a=

// a
// 1
// +1
// BINARY_ADD
// UNARY_ADD
// BINARY_ADD
// a=

// a
// +(1 + (+1))
// UNARY_ADD
// BINARY_ADD
// a=
