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

    int * arglist = malloc(argc * sizeof(int));
    for (int i = 0; i < argc; i++)
        arglist[i] = argv[i + 1];

    addVar("argv", makeList(argc, arglist, 0));

    // Interpret
    quokka_interpret(code);

    stack = realloc(stack, sizeof(int));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
        pushTopIndex(old_stack[i]);

    free(old_stack);

    free(locals.names);
    free(locals.values);

    locals.count = old_locals_count;
    locals.names = malloc((locals.count + 1) * sizeof(char *));
    locals.values = malloc((locals.count + 1) * sizeof(int));

    for (int i = 0; i < locals.count; i++)
    {
        locals.names[i] = old_locals_names[i];
        locals.values[i] = old_locals_values[i];
    }

    free(old_locals_names);
    free(old_locals_values);

    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
    can_return = old_can_return;

    if (ret_stack_size)
    {
        int isiterable = 0;

        int * ind = malloc(sizeof(int));
        ind[0] = popRetTop();
        int indlen = 1;

        Object retobj = mem[ind[0]];

        print("BEFORE : ");
        q_function_println(indlen, ind);

        // If return object is an iterable that DOES contain memory indexes,
        // add every memory index to ind so that they aren't cleaned
        if (!strcmp(retobj.name, "list"))
        {
            isiterable = 1;

            int * iterobj = objectGetAttr(retobj, "value");
            int iterlen = ((int *)objectGetAttr(retobj, "length"))[0];

            for (int i = 0; i < iterlen; i++)
            {
                pushMem(mem[iterobj[i]]);

                // mem = realloc(mem, memsize * sizeof(Object));

                // for (int i = iterobj[i]; i < memsize; i++)
                //     mem[i] = mem[i + 1];

                // memsize--;

                ind = realloc(ind, (indlen + 1) * sizeof(int));
                ind[indlen] = iterobj[i];
                indlen++;

                iterobj[i] = memsize - 1;
            }
        }

        print("BEFORE : ");
        q_function_println(indlen, ind);

        // Clean up memory after function call
        int j = -1;
        for (int i = memsize - indlen; i >= old_memsize; i--)
        {
            print(i);
            print(":");
            print(memsize);
            print(", ");
            print(intArrHas(ind, i, indlen));
            print(" : ");
            print(j);
            print(" ; <");
            print(mem[i].name);
            print("> ");
            if (strcmp(mem[i].name, "list")) // If not list
                q_function_print(1, makeIntPtr(i));
            print("\n");

            if (intArrHas(ind, i, indlen))
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

        println("yeah");

        if (isiterable)
        {
            println("IT IS ITERABLE");

            int * iterobj = objectGetAttr(mem[ind[0] - j], "value");
            int iterlen = ((int *)objectGetAttr(mem[ind[0] - j], "length"))[0];

            // Realign all the memory indexes of the iterable
            for (int i = 0; i < iterlen; i++)
            {
                iterobj[i] -= j;
            }
        }

        println("GOT HERE");

        int * ret = makeIntPtr(ind[0] - j);
        pushTrash(ret);

        print("RET : ");
        print(ind[0]);
        print(" : ");
        print(j);
        print(" ,,, ");
        println(ret[0]);

        print("AFTER : ");
        q_function_println(1, ret);

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
