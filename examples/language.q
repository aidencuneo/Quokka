// Functions
fun addProgram 1
    if type(argv[0]) != 'list'
        println('Invalid type for program, should be list')
        exit()
    end
    programs += [argv[0]]
    ret len(programs) - 1
end

fun runProgram 1
    prog = programs[argv[0]]
    init()
    line = 0
    while line < len(prog)
        a = prog[line]

        if a[0] == '@'
            pointer = int(a[1])
            global pointer
        elif a[0] == 0
            print(a[1] ? a[1] : 0)
        elif a[0] == 1
            print(tochar(int(a[1])))
        elif a[0] == 2
            stack += [int(a[1])]
        elif a[0] == 3
            if int(a[1])
                line = int(a[2]) < 1 ? 1 : int(a[2])
            end
        elif a[0] == 4
            line = int(a[1]) ? line + int(a[2]) : line
        end

        line++
    end
end

fun init
    stack = []
    global stack
end

// Variables
pointer = []
programs = []
stack = []

global pointer
global programs
global stack

// -- Programming --
//
// As an argument to any instruction, '@' can be used
// in place of a value to signify the pointer.
//
// Instructions:
//
// '@' : Set the pointer to the following value
// 0 : Print decimal value of the following value
// 1 : Print character code of the following value
// 2 : Push the following value to the stack
// 3 : Goto absolute if argument 1 is not zero (minimum goto location is 1)
// 4 : Goto relative if argument 1 is not zero (can take a negative goto location)

// User programming
addProgram([
    [0, 32],
    [1, 10],
    [2],
    [3, 1],
])

// Run init program
runProgram(0)
