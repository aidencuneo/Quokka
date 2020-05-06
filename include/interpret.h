char ** stack;

void interp_init()
{
    stack = malloc(1 * sizeof(char *));
}

char * quokka_interpret(char * bytecode)
{
    return "hello there";
}
