// Operators:
// +------------------------+
// | +  | -  | *  | /  | ** |
// +------------------------+
// | <  | >  | <= | >= | == |
// +------------------------+
// | += | -= | *= | /= | =  |
// +------------------------+

// Functions (in order of most to least complete):
// print
// println
// input
// string
// bool
// int (not complete)

i = 0
str = ''

until i == 8
    str += 'Potatoes. '
    i += 1
end

i = 0
last_ch = ' '

while last_ch
    last_ch = str[i]
    print(last_ch)
    i += 1
end

print('\n')
