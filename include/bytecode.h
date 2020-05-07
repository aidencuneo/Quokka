char * compactBytecode(char * bytecode)
{
    int linecount = charCount(bytecode, '\n');

    char ** lines = malloc(linecount);
    char * output = malloc(1);
    strcpy(output, "");

    tokenise(lines, bytecode, "\n");

    for (int i = 0; i < linecount; i++)
    {
        char ** cl = quokka_line_tok(lines[i]);

        if (!strcmp(cl[0], "STORE_NAME"))
        {
            output = realloc(output, strlen(output) + 2 + 1);
            strcat(output, "\1 ");
        }
        else if (!strcmp(cl[0], "BINARY_ADD"))
        {
            output = realloc(output, strlen(output) + 2 + 1);
            strcat(output, "\2 ");
        }
        else if (!strcmp(cl[0], "BINARY_SUB"))
        {
            output = realloc(output, strlen(output) + 2 + 1);
            strcat(output, "\2 ");
        }
        else if (!strcmp(cl[0], "BINARY_MUL"))
        {
            output = realloc(output, strlen(output) + 2 + 1);
            strcat(output, "\2 ");
        }
        else if (!strcmp(cl[0], "BINARY_DIV"))
        {
            output = realloc(output, strlen(output) + 2 + 1);
            strcat(output, "\2 ");
        }
        else
        {
            output = realloc(output, strlen(output) + strlen(cl[0]) + 1 + 1);
            strcat(output, cl[0]);
            strcat(output, " ");
        }

        int len = arrsize(cl);
        for (int p = 0; p < len; p++)
        {
            output = realloc(output, strlen(output) + strlen(cl[p]) + 1 + 1);
            strcat(output, cl[p]);
            strcat(output, " ");
        }

        output = realloc(output, strlen(output) + strlen(INSTRUCTION_END) + 1);
        strcat(output, INSTRUCTION_END);
    }

    return output;
}
