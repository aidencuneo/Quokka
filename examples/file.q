// Operators:
// +------------------------+
// | +  | -  | *  | /  | =  |
// +------------------------+
// | <  | >  | <= | >= | == |
// +------------------------+

// Functions (in order of most to least complete):
// print
// println
// input
// string
// int (not complete)

// i = input()

// if i == 'hello'
//     println('Hi.')
// elif i == 'hi'
//     println('Hello.')
// else
//     println('Oh.')
// end

i = 50
while i
    if i > 20
        println('> 20')
    elif i == 20
        println('  20')
    else
        println('< 20')
    end
    i = i - 1
end
