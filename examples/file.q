// import * (
//     'builtins',
// )

import (
    'strutil',
    'time',
)

fun str
    ret 'String'
end

i = 0x0
until!
    println('It has been', ++i, '(' + (+i) + ')', 'seconds')
    time.sleep(1)
end
