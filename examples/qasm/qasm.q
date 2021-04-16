// Qasm Assembler
// Author: Aiden Blishen Cuneo


fun stack_push 1
    stack += [argv[0]]
end


fun stack_pop
    ret stack[-1]
end


reg = []
stack = []
mem = []

global reg, stack, mem

stack_push(10)
stack_push(15)
stack_push(20)
stack_push(25)
println(stack)
println(stack_pop())
