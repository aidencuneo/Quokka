Object __call___function(int argc, int * argv)
{
    // Get function code
    char * code = objectGetAttr(mem[argv[0]], "value");

    // Set up the environment for the function call
    int old_memsize = memsize;

    int * old_stack = malloc(stack_size * sizeof(int));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;

    char ** old_locals_names = malloc(locals.count * sizeof(char *));
    int * old_locals_values = malloc(locals.count * sizeof(int));
    for (int i = 0; i < locals.count; i++)
    {
        old_locals_names[i] = locals.names[i];
        old_locals_values[i] = locals.values[i];
    }
    int old_locals_count = locals.count;

    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;
    int old_can_return = can_return;

    can_return = 1;

    // Vars
    int * intptr = makeIntPtr(argc);
    pushTrash(intptr);

    addVar("argc", makeInt(intptr));

    Object * arglist = malloc(argc * sizeof(Object));
    for (int i = 0; i < argc; i++)
        arglist[i] = objectCopy(mem[argv[i + 1]]);

    addVar("argv", makeList(argc, arglist, 0));

    // Interpret
    quokka_interpret(code);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    int ind = -1;
    int indlen = 0;

    if (ret_stack_size)
    {
        // Just peek the top item of the return stack
        ind = ret_stack[ret_stack_size - 1];
        indlen = 1;
    }

    Object retobj = mem[ind];

    print("BEFORE : ");
    q_function_println(1, makeIntPtr(ind));

    // for (int i = 0; i < memsize; i++)
    // {
    //     print(i);
    //     print(": ");
    //     q_function_println(1, makeIntPtr(i));
    // }

    // Clean up memory after function call
    int j = -1;
    for (int i = memsize - indlen; i >= old_memsize; i--)
    {
        // print(i);
        // print(":");
        // print(memsize);
        // print(", ");
        // print(i == ind);
        // print(" : ");
        // print(j);
        // print(" ; <");
        // print(mem[i].name);
        // print("> ");
        // q_function_println(1, makeIntPtr(i));

        if (i == ind)
        {
            if (j == -1)
                j = 0;
            else
            {
                popKeepMem(i);
                j++;
            }
        }
        else
        {
            popMem(i);
            if (j != -1)
                j++;
        }
    }

    for (int i = 0; i < memsize; i++)
    {
        print(i);
        print(": ");
        freeObject(q_function_println(1, makeIntPtr(i)));
    }

    // Recreate and realign previous stack
    stack = realloc(stack, sizeof(int));
    stack_size = 0;

    println("Resetting...");
    println(-j);
    for (int i = 0; i < old_stack_size; i++)
    {
        println(old_stack[i]);
        if (old_stack[i] > old_memsize)
            pushTopIndex(old_stack[i] - j);
        else
            pushTopIndex(old_stack[i]);
    }
    println("Reset.");

    free(old_stack);

    // Recreate and realign variable lists (only locals for now)
    free(locals.names);
    free(locals.values);

    locals.count = old_locals_count;
    locals.names = malloc((locals.count + 1) * sizeof(char *));
    locals.values = malloc((locals.count + 1) * sizeof(int));

    for (int i = 0; i < locals.count; i++)
    {
        locals.names[i] = old_locals_names[i];
        locals.values[i] = old_locals_values[i] - j;
    }

    free(old_locals_names);
    free(old_locals_values);

    // If there's anything to return, return it
    if (ret_stack_size)
    {
        int * ret = makeIntPtr(ind - j);
        pushTrash(ret);

        print("AFTER  : ");
        freeObject(q_function_println(1, ret));

        return makeInt(ret);
    }

    // Return null
    pushMem(makeNull());

    int * ret = makeIntPtr(memsize - 1);
    pushTrash(ret);

    return makeInt(ret);
}

Object makeFunction(char ** bytecode, int argmin, int argmax)
{
    int * argminptr = makeIntPtr(argmin);
    int * argmaxptr = makeIntPtr(argmax);
    pushTrash(argminptr);
    pushTrash(argmaxptr);

    Object self;

    self = makeObject("function", *bytecode);

    // __call__
    self = addObjectValue(self, "__call__", &__call___function);
    self = addObjectValue(self, "__call__argmin", argminptr);
    self = addObjectValue(self, "__call__argmax", argmaxptr);

    return self;
}
