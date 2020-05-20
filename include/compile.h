// Definitions:
#define SEPARATOR " "
#define INSTRUCTION_END "\n"

// Ints:
int current_line;
int scope;
int file_line_count;

// String arrays:
char ** file_tokens;

// Scope stack stuff
char ** scpstk;
int * scps;
int * scplines;
int scpstk_size;

// Function declarations
void error(char * text, int line);
int isidentifier(char * word);
int isinteger(char * word);
int islong(char * word);
int stringInList(char ** arr, char * key);
void arrlstrip(char ** line);
int stringCount(char ** lst, char * st);
int stringCountUntil(char ** lst, char * st, int len);

char * quokka_compile_line(char * linetext, int num, int lineLen, int isInline);
char * quokka_compile_line_tokens(char ** line, int num, int lineLen, int isInline);
char * quokka_compile_tokens(char ** tokens, int isInline);
char * quokka_compile_raw(char * rawtext, int maxtokensize, int isInline);
char * quokka_compile_fname(char * filename);

char ** quokka_line_tok(char * line);

void error(char * text, int line)
{
    int MAXLINE = 128;

    char * fullfile;

    if (in_cli_mode)
    {
        fullfile = cli_current_line;
        resetStack();
    }
    else
        fullfile = readfile(current_file);

    char * lineprevious = malloc(MAXLINE);
    char * linepreview = malloc(MAXLINE);
    char * linenext = malloc(MAXLINE);

    strcpy(lineprevious, "");
    strcpy(linepreview, "");
    strcpy(linenext, "");

    int c = 0;
    for (int i = 0; i < strlen(fullfile); i++)
    {
        if (fullfile[i] == '\n')
            c++;
        else if (c == line - 1 && strlen(lineprevious) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strncat(lineprevious, &fullfile[i], 1);
        else if (c == line && strlen(linepreview) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strncat(linepreview, &fullfile[i], 1);
        else if (c == line + 1 && strlen(linenext) < MAXLINE && fullfile[i] != 10 && fullfile[i] != 13)
            strncat(linenext, &fullfile[i], 1);
        if (c > line + 1)
            break;
    }

    line++;

    println("\nProgram execution terminated:\n");

    printf("At %s : Line %d\n\n", current_file, line);

    if (line - 1 > 0)
        printf("  %d | %s\n", line - 1, lineprevious);
    printf("> %d | %s\n", line, linepreview);
    if (line < charCount(fullfile, '\n') + 1)
        printf("  %d | %s\n", line + 1, linenext);
    print("\n");

    printf("Error: %s\n\n", text);

    if (!in_cli_mode)
        free(fullfile);

    free(lineprevious);
    free(linepreview);
    free(linenext);

    // Exit and free the stack (if not in CLI mode)
    if (!in_cli_mode)
    {
        resetStack();
        exit(1);
    }
}

int isidentifier(char * word)
{
    int size = strlen(word);
    int hasnotnum = 0;

    for (int i = 0; i < size; i++)
    {
        if (!isalpha(word[i]) &&
            word[i] != '_' &&
            !(hasnotnum && isdigit(word[i])))
            return 0;
        if (isalpha(word[i]) || word[i] == '_')
            hasnotnum = 1;
    }

    return 1;
}

int isinteger(char * word)
{
    int size = strlen(word);

    for (int i = 0; i < size; i++)
    {
        if (!isdigit(word[i]))
            return 0;
    }

    return 1;
}

int islong(char * word)
{
    int size = strlen(word);

    if (size <= 1)
        return 0;

    if (word[size - 1] != 'l' && word[size - 1] != 'L')
        return 0;

    for (int i = 0; i < size - 1; i++)
    {
        if (!isdigit(word[i]))
            return 0;
    }

    return 1;
}

int stringInList(char ** arr, char * key)
{
    int len = arrsize(arr);

    for (int n = 0; arr[n] != NULL; ++n)
    {
        if (strcmp(arr[n], key) == 0)
            return 1;
    }

    return 0;
}

void arrlstrip(char ** line)
{
    int n = arrsize(line);

    for (int c = 0; c < n - 1; c++)
        line[c] = line[c + 1];

    line[n - 1] = "";
}

void arrdelindex(char ** line, int index)
{
    int n = arrsize(line);

    for (int c = index; c < n - 1; c++)
        line[c] = line[c + 1];

    line[n - 1] = "";
}

int stringCount(char ** lst, char * st)
{
    int len = arrsize(lst);
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (!strcmp(lst[i], st))
            count++;
    }

    return count;
}

int stringCountUntil(char ** lst, char * st, int len)
{
    int count = 0;

    for (int i = 0; i < len && lst[i] != NULL; i++)
    {
        if (!strcmp(lst[i], st))
            count++;
    }

    return count;
}

void scpstkPush(char * kw, int scp, int linenum)
{
    scpstk = realloc(scpstk, (scpstk_size + 1) * sizeof(char *));
    scpstk[scpstk_size] = kw;

    scps = realloc(scps, (scpstk_size + 1) * sizeof(int));
    scps[scpstk_size] = scp;

    scplines = realloc(scplines, (scpstk_size + 1) * sizeof(int));
    scplines[scpstk_size] = linenum;

    scpstk_size++;
}

void scpstkPop()
{
    scpstk = realloc(scpstk, (scpstk_size + 1) * sizeof(char *));
    scps = realloc(scps, (scpstk_size + 1) * sizeof(int));
    scplines = realloc(scplines, (scpstk_size + 1) * sizeof(int));

    scpstk_size--;
}

int findNextIfChain(char * kwtype, int cur_line, int scp)
{
    // Index will be -1 if 'end' not found
    int ind = -2;
    int tempscope = scp;
    int blanks = 0; // Num of consecutive blank lines

    // 0 = 'if'
    // 1 = 'elif'
    // 2 = 'else'
    int kw;

    if (!strcmp(kwtype, "if"))
        kw = 0;
    else if (!strcmp(kwtype, "elif"))
        kw = 1;
    else if (!strcmp(kwtype, "else"))
        kw = 2;

    for (int i = cur_line + 1; i < file_line_count; i++)
    {
        char ** templine = quokka_line_tok(file_tokens[i]);

        if (!arrsize(templine))
        {
            free(templine);
            continue;
        }

        // Error checking
        if (tempscope == scp)
        {
            if (kw == 1 && !strcmp(templine[0], "if"))
            {
                error("if statement after elif statement in the same if chain",
                    current_line);
            }
            else if (kw == 2 && !strcmp(templine[0], "if"))
            {
                error("if statement after else statement in the same if chain",
                    current_line);
            }
            else if (kw == 2 && !strcmp(templine[0], "elif"))
            {
                error("elif statement after else statement in the same if chain",
                    current_line);
            }
            else if (kw == 2 && !strcmp(templine[0], "else"))
            {
                error("else statement on the end of another else statement",
                    current_line);
            }
        }

        // Regular scope checking and all the rest
        if (!strcmp(templine[0], "if") ||
            !strcmp(templine[0], "while") ||
            !strcmp(templine[0], "until") ||
            !strcmp(templine[0], "fun") ||
            !strcmp(templine[0], "for"))
        {
            tempscope++;
        }
        else
        {
            if (!strcmp(templine[0], "end") ||
               (!strcmp(templine[0], "elif") && (kw == 0 || kw == 1)) ||
               (!strcmp(templine[0], "else") && (kw == 0 || kw == 1)))
            {
                if (tempscope == scp)
                {
                    ind = i;
                    break;
                }
            }
            if (!strcmp(templine[0], "end"))
                tempscope--;
        }

        free(templine);
    }

    return ind + 1;
}

int findNextEnd(char * kwtype, int cur_line, int scp)
{
    // Index will be -1 if 'end' not found
    int ind = -2;
    int tempscope = scp;
    int blanks = 0; // Num of consecutive blank lines

    for (int i = cur_line + 1; i < file_line_count; i++)
    {
        char ** templine = quokka_line_tok(file_tokens[i]);

        if (!arrsize(templine))
        {
            free(templine);
            continue;
        }

        if (!strcmp(templine[0], "if") ||
            !strcmp(templine[0], "while") ||
            !strcmp(templine[0], "until") ||
            !strcmp(templine[0], "fun") ||
            !strcmp(templine[0], "for"))
        {
            tempscope++;
        }
        else if (!strcmp(templine[0], "end"))
        {
            if (tempscope == scp)
            {
                ind = i;
                break;
            }
            else tempscope--;
        }

        free(templine);
    }

    return ind + 1;
}

int compile_comma_list(char *** outptr, char ** comma_list)
{
    outptr[0] = comma_list;

    int templen = arrsize(outptr[0]);
    int lastwascomma = 0;
    for (int i = 0; i < templen; i++)
    {
        if (!strcmp(outptr[0][i], ","))
        {
            if (lastwascomma)
            {
                arrdelindex(outptr[0], i);
                templen--;
            }
            lastwascomma = 1;
        }
        else lastwascomma = 0;
    }
    if (templen)
        while (!strcmp(outptr[0][templen - 1], ","))
            templen--;

    return templen;
}

int compile_comma_list_string(char *** outptr, char * comma_string)
{
    char ** comma_list = quokka_line_tok(comma_string);
    int templen = compile_comma_list(outptr, comma_list);

    return templen;
}

void compile_init()
{
    scope = 0;
}

char * quokka_compile_line(char * linetext, int num, int lineLen, int isInline)
{
    char ** line = quokka_line_tok(linetext);

    return quokka_compile_line_tokens(line, num, lineLen, isInline);
}

char * quokka_compile_line_tokens(char ** line, int num, int lineLen, int isInline)
{
    char * bytecode = malloc(1);
    bytecode[0] = '\0';
    //strcpy(bytecode, "");

    int len;
    if (lineLen == -1)
        len = arrsize(line);
    else len = lineLen;

    if (!len)
    {
        return bytecode;
    }

    if (len < 2)
        line[1] = "";

    if (verbose) println(line[0]);

    if (stringInList(line, "="))
    {
        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (isInline)
            error("variables must be defined at the start of a line", num);

        if (!isidentifier(line[0]))
            error("variable name to assign must be a valid identifier", num);

        if (strcmp(line[1], "="))
            error("invalid syntax", num);

        if (len < 3)
            error("variable definition missing variable value", num);

        char * varname = strndup(line[0], strlen(line[0]));

        arrlstrip(line);
        arrlstrip(line);
        len -= 2;
        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        mstrcat(&bytecode, "STORE_NAME");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, varname);
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (stringInList(line, "+="))
    {
        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (isInline)
            error("variables must be defined at the start of a line", num);

        if (!isidentifier(line[0]))
            error("variable name to assign must be a valid identifier", num);

        if (strcmp(line[1], "+="))
            error("invalid syntax", num);

        if (len < 3)
            error("variable definition missing variable value", num);

        char * varname = strndup(line[0], strlen(line[0]));

        char ** templine = malloc(5 * sizeof(char *));
        int size = 4;

        templine[0] = varname;
        templine[1] = "=";
        templine[2] = varname;
        templine[3] = "+";

        for (int i = 2; i < len; i++)
        {
            templine = realloc(templine, (size + 1) * sizeof(char *));
            templine[size] = line[i];
            size++;
        }

        templine = realloc(templine, (size + 1) * sizeof(char *));
        templine[size] = NULL;

        char * temp = quokka_compile_line_tokens(templine, num, size, 0);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
    }
    else if (stringInList(line, "-="))
    {
        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (isInline)
            error("variables must be defined at the start of a line", num);

        if (!isidentifier(line[0]))
            error("variable name to assign must be a valid identifier", num);

        if (strcmp(line[1], "-="))
            error("invalid syntax", num);

        if (len < 3)
            error("variable definition missing variable value", num);

        char * varname = strndup(line[0], strlen(line[0]));

        char ** templine = malloc(5 * sizeof(char *));
        int size = 4;

        templine[0] = varname;
        templine[1] = "=";
        templine[2] = varname;
        templine[3] = "-";

        for (int i = 2; i < len; i++)
        {
            templine = realloc(templine, (size + 1) * sizeof(char *));
            templine[size] = line[i];
            size++;
        }

        templine = realloc(templine, (size + 1) * sizeof(char *));
        templine[size] = NULL;

        char * temp = quokka_compile_line_tokens(templine, num, size, 0);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
    }
    else if (stringInList(line, "*="))
    {
        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (isInline)
            error("variables must be defined at the start of a line", num);

        if (!isidentifier(line[0]))
            error("variable name to assign must be a valid identifier", num);

        if (strcmp(line[1], "*="))
            error("invalid syntax", num);

        if (len < 3)
            error("variable definition missing variable value", num);

        char * varname = strndup(line[0], strlen(line[0]));

        char ** templine = malloc(5 * sizeof(char *));
        int size = 4;

        templine[0] = varname;
        templine[1] = "=";
        templine[2] = varname;
        templine[3] = "*";

        for (int i = 2; i < len; i++)
        {
            templine = realloc(templine, (size + 1) * sizeof(char *));
            templine[size] = line[i];
            size++;
        }

        templine = realloc(templine, (size + 1) * sizeof(char *));
        templine[size] = NULL;

        char * temp = quokka_compile_line_tokens(templine, num, size, 0);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
    }
    else if (stringInList(line, "/="))
    {
        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (isInline)
            error("variables must be defined at the start of a line", num);

        if (!isidentifier(line[0]))
            error("variable name to assign must be a valid identifier", num);

        if (strcmp(line[1], "/="))
            error("invalid syntax", num);

        if (len < 3)
            error("variable definition missing variable value", num);

        char * varname = strndup(line[0], strlen(line[0]));

        char ** templine = malloc(5 * sizeof(char *));
        int size = 4;

        templine[0] = varname;
        templine[1] = "=";
        templine[2] = varname;
        templine[3] = "/";

        for (int i = 2; i < len; i++)
        {
            templine = realloc(templine, (size + 1) * sizeof(char *));
            templine[size] = line[i];
            size++;
        }

        templine = realloc(templine, (size + 1) * sizeof(char *));
        templine[size] = NULL;

        char * temp = quokka_compile_line_tokens(templine, num, size, 0);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
    }
    else if (!strcmp(line[0], "while"))
    {
        if (isInline)
            error("while loop can only be at the start of a line", num);
        if (len <= 1)
            error("while loop requires a condition", num);

        // Set new line
        char * numstr = malloc(11);
        strcpy(numstr, "");
        snprintf(numstr, 10, "%d", current_line + 1);

        mstrcat(&bytecode, numstr);
        mstrcat(&bytecode, INSTRUCTION_END);

        arrlstrip(line);
        len--;

        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        int next = findNextEnd("while", num, scope);
        if (next == -1)
            error("while loop missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_IF_FALSE");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(next));
        mstrcat(&bytecode, INSTRUCTION_END);

        scpstkPush("while", scope, current_line + 1);

        scope++;
    }
    else if (!strcmp(line[0], "until"))
    {
        if (isInline)
            error("until loop can only be at the start of a line", num);
        if (len <= 1)
            error("until loop requires a condition", num);

        // Set new line
        char * numstr = malloc(11);
        strcpy(numstr, "");
        snprintf(numstr, 10, "%d", current_line + 1);

        mstrcat(&bytecode, numstr);
        mstrcat(&bytecode, INSTRUCTION_END);

        arrlstrip(line);
        len--;

        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        int next = findNextEnd("until", num, scope);
        if (next == -1)
            error("until loop missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_IF_TRUE");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(next));
        mstrcat(&bytecode, INSTRUCTION_END);

        scpstkPush("until", scope, current_line + 1);

        scope++;
    }
    else if (!strcmp(line[0], "if"))
    {
        if (isInline)
            error("if statement can only be at the start of a line", num);
        if (len <= 1)
            error("if statement requires a condition", num);

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        arrlstrip(line);
        len--;

        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        int next = findNextIfChain("if", num, scope);
        if (next == -1)
            error("if statement missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_IF_FALSE");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(next));
        mstrcat(&bytecode, INSTRUCTION_END);

        scope++;
    }
    else if (!strcmp(line[0], "elif"))
    {
        if (isInline)
            error("elif statement can only be at the start of a line", num);
        if (len <= 1)
            error("elif statement requires a condition", num);

        int nextend = findNextEnd("elif", num, scope);
        if (nextend == -1)
            error("elif statement missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_TO");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(nextend));
        mstrcat(&bytecode, INSTRUCTION_END);

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        arrlstrip(line);
        len--;

        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        int next = findNextIfChain("elif", num, scope);
        if (next == -1)
            error("elif statement missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_IF_FALSE");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(next));
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (!strcmp(line[0], "else"))
    {
        if (isInline)
            error("else statement can only be at the start of a line", num);
        if (len > 1)
            error("else statement does not take arguments", num);

        int nextend = findNextEnd("else", num, scope);
        if (nextend == -1)
            error("else statement missing 'end' keyword", num);

        mstrcat(&bytecode, "JUMP_TO");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, intToStr(nextend));
        mstrcat(&bytecode, INSTRUCTION_END);

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);
    }
    else if (!strcmp(line[0], "end"))
    {
        if (isInline)
            error("'end' can only be at the start of a line", num);

        if (len > 1)
            error("'end' does not take arguments", num);

        if (scpstk_size)
        {
            char * kw = scpstk[scpstk_size - 1];
            int scp = scps[scpstk_size - 1];
            int scpline = scplines[scpstk_size - 1];

            if (scope - 1 == scp)
            {
                if (!strcmp(kw, "while") || !strcmp(kw, "until"))
                {
                    mstrcat(&bytecode, "JUMP_BACK");
                    mstrcat(&bytecode, SEPARATOR);
                    mstrcat(&bytecode, intToStr(scpline));
                    mstrcat(&bytecode, INSTRUCTION_END);

                    scpstkPop();
                }
            }
        }

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        scope--;
    }
    else if (!strcmp(line[0], "fun"))
    {
        if (isInline)
            error("function definition can only be at the start of a line", num);
        if (len <= 1)
            error("function definition requires at least a function name", num);
        if (len > 4)
            error("function definition received too many arguments", num);

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        int next = findNextEnd("fun", num, scope);
        if (next == -1)
            error("function definition missing 'end' keyword", num);

        char * funcname = line[1];
        int argmin = 0;
        int argmax = 0;

        if (len > 3)
        {
            if (!strcmp(line[2], "*"))
                argmin = 0;
            else
                argmin = strtol(line[2], NULL, 10);

            if (!strcmp(line[3], "*"))
                argmax = -1;
            else
                argmax = strtol(line[3], NULL, 10);
        }
        else if (len > 2)
        {
            if (!strcmp(line[2], "*"))
            {
                argmin = 0;
                argmax = -1;
            }
            else
            {
                argmin = strtol(line[2], NULL, 10);
                argmax = argmin;
            }
        }

        char * funcbytecode = malloc(1);
        strcpy(funcbytecode, "");

        for (current_line = num + 1; current_line < next; current_line++)
        {
            char * bc = strReplace(
                quokka_compile_line(file_tokens[current_line], current_line, -1, 0),
                "\n", "\t");

            mstrcat(&funcbytecode, bc);
        }

        num = next;

        if (argmin < 0 && argmin != -1)
            error("minimum argument count for function definition can not be below 0", num);
        if (argmax < argmin && argmax != -1)
            error("maximum argument count for function definition can not be below minimum argument count",
                num);

        mstrcat(&bytecode, "DEFINE_FUNCTION");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, funcname);
        mstrcat(&bytecode, SEPARATOR);

        if (argmin == -1)
            mstrcat(&bytecode, "*");
        else
            mstrcat(&bytecode, intToStr(argmin));

        mstrcat(&bytecode, SEPARATOR);

        if (argmax == -1)
            mstrcat(&bytecode, "*");
        else
            mstrcat(&bytecode, intToStr(argmax));

        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, "[");
        mstrcat(&bytecode, funcbytecode);
        mstrcat(&bytecode, "]");
        mstrcat(&bytecode, INSTRUCTION_END);

        scope++;
    }
    else if (!strcmp(line[0], "del"))
    {
        if (isInline)
            error("del statement must be at the start of a line", num);

        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        for (int i = 1; i < len; i++)
        {
            if (!strcmp(line[i], ","))
                continue;

            if (!isidentifier(line[i]))
                error("invalid syntax", num);

            mstrcat(&bytecode, "DEL_VAR");
            mstrcat(&bytecode, SEPARATOR);
            mstrcat(&bytecode, line[i]);
            mstrcat(&bytecode, INSTRUCTION_END);
        }
    }
    else if (!strcmp(line[0], "ret"))
    {
        if (isInline)
            error("ret statement must be at the start of a line", num);

        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        arrlstrip(line);
        len--;

        char * temp = quokka_compile_line_tokens(line, num, len, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        mstrcat(&bytecode, "RETURN");
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (stringInList(line, ","))
    {
        char * latestvalue = malloc(1);
        strcpy(latestvalue, "");

        int lastwascomma = 0;

        for (int p = 0; p < len; p++)
        {
            if (!strlen(line[p]))
                break;

            if (!strcmp(line[p], ","))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                mstrcat(&bytecode, strndup(temp, strlen(temp)));
                free(temp);

                latestvalue = realloc(latestvalue, 1);
                strcpy(latestvalue, "");

                lastwascomma = 1;
            }
            else
            {
                latestvalue = realloc(latestvalue, strlen(latestvalue) + strlen(line[p]) + 1 + 1);
                strcat(latestvalue, line[p]);
                strcat(latestvalue, " ");

                lastwascomma = 0;
            }
        }

        if (strlen(latestvalue))
        {
            char * temp = quokka_compile_line(latestvalue, num, -1, 1);
            mstrcat(&bytecode, strndup(temp, strlen(temp)));
            free(temp);
        }

        free(latestvalue);
    }
    else if (stringInList(line, "<") || stringInList(line, ">") || stringInList(line, "<=") || stringInList(line, ">=") || stringInList(line, "=="))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        char * operslist = malloc(1);
        strcpy(operslist, "");

        char * valuelist = malloc(1);
        strcpy(valuelist, "");

        char * latestvalue = malloc(1);
        strcpy(latestvalue, "");

        int lastwasop = 1;

        for (int i = 0; i < len; i++)
        {
            if (!strcmp(line[i], "<"))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("invalid syntax at '<'", num);

                operslist = realloc(operslist, strlen(operslist) + 6 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "CMP_LT");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else if (!strcmp(line[i], ">"))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("invalid syntax at '>'", num);

                operslist = realloc(operslist, strlen(operslist) + 6 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "CMP_GT");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else if (!strcmp(line[i], "<="))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("invalid syntax at '<='", num);

                operslist = realloc(operslist, strlen(operslist) + 6 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "CMP_LE");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else if (!strcmp(line[i], ">="))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("invalid syntax at '>='", num);

                operslist = realloc(operslist, strlen(operslist) + 6 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "CMP_GE");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else if (!strcmp(line[i], "=="))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("invalid syntax at '=='", num);

                operslist = realloc(operslist, strlen(operslist) + 6 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "CMP_EQ");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else
            {
                if (lastwasop)
                {
                    latestvalue = realloc(latestvalue, strlen(line[i]) + 1 + 1);
                    memset(latestvalue, 0, strlen(latestvalue) + 1);
                    strcpy(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }
                else
                {
                    latestvalue = realloc(latestvalue, strlen(line[i]) + 1 + 1);
                    strcat(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }

                lastwasop = 0;
            }
        }

        char * temp = quokka_compile_line(latestvalue, num, -1, 1);
        valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
        strcat(valuelist, strndup(temp, strlen(temp)));
        free(temp);

        mstrcat(&bytecode, strdup(valuelist));
        mstrcat(&bytecode, strdup(operslist));

        free(valuelist);
        free(operslist);
    }
    else if (stringInList(line, "+") || stringInList(line, "-"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        char * operslist = malloc(1);
        strcpy(operslist, "");

        char * valuelist = malloc(1);
        strcpy(valuelist, "");

        char * latestvalue = malloc(1);
        strcpy(latestvalue, "");

        int lastwasop = 1;
        int lastwasunary = 0;

        int i;
        for (i = 0; i < len; i++)
        {
            if (!strcmp(line[i], "+"))
            {
                if (i + 1 >= len)
                    error("'+' missing second argument", num);

                if (lastwasop)
                {
                    // Used for unaries beginning with '+', for example: '++--+'
                    char * tmp = malloc(1);
                    strcpy(tmp, "");

                    for (i = i; i < len; i++)
                    {
                        if (!strcmp(line[i], "+"))
                        {
                            tmp = realloc(tmp, strlen(tmp) + 9 + strlen(INSTRUCTION_END) + 1);
                            strcat(tmp, "UNARY_ADD");
                            strcat(tmp, INSTRUCTION_END);
                        }
                        else if (!strcmp(line[i], "-"))
                        {
                            tmp = realloc(tmp, strlen(tmp) + 9 + strlen(INSTRUCTION_END) + 1);
                            strcat(tmp, "UNARY_SUB");
                            strcat(tmp, INSTRUCTION_END);
                        }
                        else break;
                    }

                    char * temp = quokka_compile_line(line[i], num, -1, 1);
                    valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + strlen(tmp) + 1);

                    strcat(valuelist, strndup(temp, strlen(temp)));
                    free(temp);

                    strcat(valuelist, strndup(tmp, strlen(tmp)));
                    free(tmp);

                    latestvalue = realloc(latestvalue, 1);
                    memset(latestvalue, 0, 1);
                    strcpy(latestvalue, "");

                    lastwasop = 0;
                    lastwasunary = 1;
                }
                else
                {
                    char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                    valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                    strcat(valuelist, strdup(temp));
                    free(temp);

                    latestvalue = realloc(latestvalue, 1);
                    memset(latestvalue, 0, 1);
                    strcpy(latestvalue, "");

                    operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

                    char * tmp = strdup(operslist);

                    memset(operslist, 0, strlen(operslist));
                    strcpy(operslist, "BINARY_ADD");
                    strcat(operslist, INSTRUCTION_END);
                    strcat(operslist, tmp);

                    free(tmp);

                    lastwasop = 1;
                    lastwasunary = 0;
                }
            }
            else if (!strcmp(line[i], "-"))
            {
                if (i + 1 >= len)
                    error("'-' missing second argument", num);

                if (lastwasop)
                {
                    // Used for unaries beginning with '-', for example: '-+-+'
                    char * tmp = malloc(1);
                    strcpy(tmp, "");

                    for (i = i; i < len; i++)
                    {
                        if (!strcmp(line[i], "+"))
                        {
                            tmp = realloc(tmp, strlen(tmp) + 9 + strlen(INSTRUCTION_END) + 1);
                            strcat(tmp, "UNARY_ADD");
                            strcat(tmp, INSTRUCTION_END);
                        }
                        else if (!strcmp(line[i], "-"))
                        {
                            tmp = realloc(tmp, strlen(tmp) + 9 + strlen(INSTRUCTION_END) + 1);
                            strcat(tmp, "UNARY_SUB");
                            strcat(tmp, INSTRUCTION_END);
                        }
                        else break;
                    }

                    char * temp = quokka_compile_line(line[i], num, -1, 1);
                    valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + strlen(tmp) + 1);

                    strcat(valuelist, strndup(temp, strlen(temp)));
                    free(temp);

                    strcat(valuelist, strndup(tmp, strlen(tmp)));
                    free(tmp);

                    latestvalue = realloc(latestvalue, 1);
                    memset(latestvalue, 0, 1);
                    strcpy(latestvalue, "");

                    lastwasop = 0;
                    lastwasunary = 1;
                }
                else
                {
                    char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                    valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                    strcat(valuelist, strdup(temp));
                    free(temp);

                    latestvalue = realloc(latestvalue, 1);
                    memset(latestvalue, 0, 1);
                    strcpy(latestvalue, "");

                    operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

                    char * tmp = strdup(operslist);

                    memset(operslist, 0, strlen(operslist));
                    strcpy(operslist, "BINARY_SUB");
                    strcat(operslist, INSTRUCTION_END);
                    strcat(operslist, tmp);

                    free(tmp);

                    lastwasop = 1;
                    lastwasunary = 0;
                }
            }
            // else if (!strcmp(line[i], "*"))
            // {
            //     char * temp = quokka_compile_line(latestvalue, num, -1, 1);
            //     valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
            //     strcat(valuelist, strdup(temp));
            //     free(temp);

            //     if (lastwasop)
            //         error("'*' missing first argument", num);

            //     if (i + 1 >= len)
            //         error("'*' missing second argument", num);

            //     latestvalue = realloc(latestvalue, 1);
            //     memset(latestvalue, 0, 1);

            //     operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

            //     char * tmp = strdup(operslist);

            //     memset(operslist, 0, strlen(operslist));
            //     strcpy(operslist, "BINARY_MUL");
            //     strcat(operslist, INSTRUCTION_END);
            //     strcat(operslist, tmp);

            //     free(tmp);

            //     lastwasop = 1;
            //     lastwasunary = 0;
            // }
            // else if (!strcmp(line[i], "/"))
            // {
            //     char * temp = quokka_compile_line(latestvalue, num, -1, 1);
            //     valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
            //     strcat(valuelist, strdup(temp));
            //     free(temp);

            //     if (lastwasop)
            //         error("'/' missing first argument", num);

            //     if (i + 1 >= len)
            //         error("'/' missing second argument", num);

            //     latestvalue = realloc(latestvalue, 1);
            //     memset(latestvalue, 0, 1);

            //     operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

            //     char * tmp = strdup(operslist);

            //     memset(operslist, 0, strlen(operslist));
            //     strcpy(operslist, "BINARY_DIV");
            //     strcat(operslist, INSTRUCTION_END);
            //     strcat(operslist, tmp);

            //     free(tmp);

            //     lastwasop = 1;
            //     lastwasunary = 0;
            // }
            // else if (!strcmp(line[i], "**"))
            // {
            //     char * temp = quokka_compile_line(latestvalue, num, -1, 1);
            //     valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
            //     strcat(valuelist, strdup(temp));
            //     free(temp);

            //     if (lastwasop)
            //         error("'**' missing first argument", num);

            //     if (i + 1 >= len)
            //         error("'**' missing second argument", num);

            //     latestvalue = realloc(latestvalue, 1);
            //     memset(latestvalue, 0, 1);

            //     operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

            //     char * tmp = strdup(operslist);

            //     memset(operslist, 0, strlen(operslist));
            //     strcpy(operslist, "BINARY_POW");
            //     strcat(operslist, INSTRUCTION_END);
            //     strcat(operslist, tmp);

            //     free(tmp);

            //     lastwasop = 1;
            //     lastwasunary = 0;
            // }
            else
            {
                if (lastwasop)
                {
                    latestvalue = realloc(latestvalue, strlen(line[i]) + 1 + 1);
                    memset(latestvalue, 0, strlen(latestvalue) + 1);
                    strcpy(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }
                else
                {
                    latestvalue = realloc(latestvalue, strlen(latestvalue) + strlen(line[i]) + 1 + 1);
                    strcat(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }

                lastwasop = 0;
            }
        }

        if (!lastwasop && strlen(latestvalue) && lastwasunary)
            error("invalid syntax", num);

        char * temp = quokka_compile_line(latestvalue, num, -1, 1);
        valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
        strcat(valuelist, strndup(temp, strlen(temp)));
        free(temp);

        mstrcat(&bytecode, strndup(valuelist, strlen(valuelist)));
        mstrcat(&bytecode, strndup(operslist, strlen(operslist)));

        free(valuelist);
        free(operslist);
    }
    else if (stringInList(line, "*") || stringInList(line, "/"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        char * operslist = malloc(1);
        strcpy(operslist, "");

        char * valuelist = malloc(1);
        strcpy(valuelist, "");

        char * latestvalue = malloc(1);
        strcpy(latestvalue, "");

        int lastwasop = 1;

        int i;
        for (i = 0; i < len; i++)
        {
            if (!strcmp(line[i], "*"))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("'*' missing first argument", num);

                if (i + 1 >= len)
                    error("'*' missing second argument", num);

                latestvalue = realloc(latestvalue, 1);
                memset(latestvalue, 0, 1);

                operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "BINARY_MUL");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else if (!strcmp(line[i], "/"))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("'/' missing first argument", num);

                if (i + 1 >= len)
                    error("'/' missing second argument", num);

                latestvalue = realloc(latestvalue, 1);
                memset(latestvalue, 0, 1);

                operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "BINARY_DIV");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else
            {
                if (lastwasop)
                {
                    latestvalue = realloc(latestvalue, strlen(line[i]) + 1 + 1);
                    memset(latestvalue, 0, strlen(latestvalue) + 1);
                    strcpy(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }
                else
                {
                    latestvalue = realloc(latestvalue, strlen(latestvalue) + strlen(line[i]) + 1 + 1);
                    strcat(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }

                lastwasop = 0;
            }
        }

        char * temp = quokka_compile_line(latestvalue, num, -1, 1);
        valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
        strcat(valuelist, strndup(temp, strlen(temp)));
        free(temp);

        mstrcat(&bytecode, strndup(valuelist, strlen(valuelist)));
        mstrcat(&bytecode, strndup(operslist, strlen(operslist)));

        free(valuelist);
        free(operslist);
    }
    else if (stringInList(line, "**"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        char * operslist = malloc(1);
        strcpy(operslist, "");

        char * valuelist = malloc(1);
        strcpy(valuelist, "");

        char * latestvalue = malloc(1);
        strcpy(latestvalue, "");

        int lastwasop = 1;

        int i;
        for (i = 0; i < len; i++)
        {
            if (!strcmp(line[i], "**"))
            {
                char * temp = quokka_compile_line(latestvalue, num, -1, 1);
                valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
                strcat(valuelist, strdup(temp));
                free(temp);

                if (lastwasop)
                    error("'**' missing first argument", num);

                if (i + 1 >= len)
                    error("'**' missing second argument", num);

                latestvalue = realloc(latestvalue, 1);
                memset(latestvalue, 0, 1);

                operslist = realloc(operslist, strlen(operslist) + 10 + strlen(INSTRUCTION_END) + 1);

                char * tmp = strdup(operslist);

                memset(operslist, 0, strlen(operslist));
                strcpy(operslist, "BINARY_POW");
                strcat(operslist, INSTRUCTION_END);
                strcat(operslist, tmp);

                free(tmp);

                lastwasop = 1;
            }
            else
            {
                if (lastwasop)
                {
                    latestvalue = realloc(latestvalue, strlen(line[i]) + 1 + 1);
                    memset(latestvalue, 0, strlen(latestvalue) + 1);
                    strcpy(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }
                else
                {
                    latestvalue = realloc(latestvalue, strlen(latestvalue) + strlen(line[i]) + 1 + 1);
                    strcat(latestvalue, line[i]);
                    strcat(latestvalue, " ");
                }

                lastwasop = 0;
            }
        }

        char * temp = quokka_compile_line(latestvalue, num, -1, 1);
        valuelist = realloc(valuelist, strlen(valuelist) + strlen(temp) + 1);
        strcat(valuelist, strndup(temp, strlen(temp)));
        free(temp);

        mstrcat(&bytecode, strndup(valuelist, strlen(valuelist)));
        mstrcat(&bytecode, strndup(operslist, strlen(operslist)));

        free(valuelist);
        free(operslist);
    }
    /* Will fix this another time

    else if (startswith(line[len - 1], "."))
    {
        char * templine = malloc(1);
        int is_first_item = 1;

        for (int i = 0; i < len; i++)
        {
            if (startswith(line[i], "."))
            {
                char ** temp = quokka_line_tok(line[i]);
                println("get attr");
            }
            else println(line[i]);
        }

        exit(1);

        char * temp = quokka_compile_line(templine, num, -1, 1);
        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
        free(templine);

        mstrcat(&bytecode, "GET_ATTR");
        mstrcat(&bytecode, INSTRUCTION_END);
    }

    */
    else if (startswith(line[0], "(") && endswith(line[0], ")"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (len > 1)
            error("invalid syntax", num);

        // Split up the argument list into it's elements
        char ** templine;
        int templen = compile_comma_list_string(&templine, strSlice(line[0], 1, 1));

        char * temp = quokka_compile_line_tokens(templine, num, templen, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);
        free(templine);
    }
    else if (isidentifier(line[0]) && startswith(line[1], "(") && endswith(line[1], ")"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (len > 2)
            error("invalid syntax", num);

        mstrcat(&bytecode, "LOAD_NAME");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, strndup(line[0], strlen(line[0])));
        mstrcat(&bytecode, INSTRUCTION_END);

        // If arguments were given to the function
        if (strlen(line[1]) > 2)
        {
            // Split up the argument list into it's elements
            char ** templine;
            int templen = compile_comma_list_string(&templine, strSlice(line[1], 1, 1));

            char * temp = quokka_compile_line_tokens(templine, num, templen, 1);

            mstrcat(&bytecode, strndup(temp, strlen(temp)));

            free(temp);

            mstrcat(&bytecode, "CALL_FUNCTION");
            mstrcat(&bytecode, SEPARATOR);
            mstrcat(&bytecode, intToStr(stringCountUntil(templine, ",", templen) + 1));
            mstrcat(&bytecode, INSTRUCTION_END);

            free(templine);
        }
        // If no arguments were given
        else
        {
            mstrcat(&bytecode, "CALL_FUNCTION");
            mstrcat(&bytecode, SEPARATOR);
            mstrcat(&bytecode, "0");
            mstrcat(&bytecode, INSTRUCTION_END);
        }
    }
    else if (islong(line[0]) && len == 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        // Clear leading 0's on integers
        while (startswith(line[0], "0") && strlen(line[0]) > 2)
            line[0]++;

        mstrcat(&bytecode, "LOAD_LONG");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, line[0]);
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (isinteger(line[0]) && len == 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        // Clear leading 0's on integers
        while (startswith(line[0], "0") && strlen(line[0]) > 1)
            line[0]++;

        if (strlen(line[0]) > 10)
            mstrcat(&bytecode, "LOAD_LONG");
        else
            mstrcat(&bytecode, "LOAD_INT");

        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, line[0]);
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (!strcmp(line[0], "null") && len == 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        mstrcattrip(&bytecode, "LOAD_NULL", INSTRUCTION_END);
    }
    else if ((
        (startswith(line[0], "'") && endswith(line[0], "'")) ||
        (startswith(line[0], "\"") && endswith(line[0], "\""))
    ) && len == 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        mstrcat(&bytecode, "LOAD_STRING");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, line[0]);
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (isidentifier(line[0]) && len == 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        mstrcat(&bytecode, "LOAD_NAME");
        mstrcat(&bytecode, SEPARATOR);
        mstrcat(&bytecode, line[0]);
        mstrcat(&bytecode, INSTRUCTION_END);
    }
    else if (startswith(line[len - 1], "[") && endswith(line[len - 1], "]") && len > 1)
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (len > 2)
            error("invalid syntax", num);

        char * indexarg = strndup(line[len - 1], strlen(line[len - 1]));

        char * temp = quokka_compile_line_tokens(line, num, len - 1, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        // If index received an item, for example: [0]
        if (strlen(indexarg) > 2)
        {
            char * temp = quokka_compile_line(strSlice(indexarg, 1, 1), num, -1, 1);

            mstrcat(&bytecode, strndup(temp, strlen(temp)));

            free(temp);

            mstrcat(&bytecode, "GET_INDEX");
            mstrcat(&bytecode, SEPARATOR);
            mstrcat(&bytecode, "1");
            mstrcat(&bytecode, INSTRUCTION_END);
        }
        // If nothing was given, for example: []
        else
        {
            mstrcat(&bytecode, "GET_INDEX");
            mstrcat(&bytecode, SEPARATOR);
            mstrcat(&bytecode, "*");
            mstrcat(&bytecode, INSTRUCTION_END);
        }
    }
    else if (startswith(line[0], "[") && endswith(line[0], "]"))
    {
        // Set new line
        if (!isInline)
            mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        if (len > 1)
            error("invalid syntax", num);

        // Split up the list into it's elements
        char ** templine;
        int templen = compile_comma_list_string(&templine, strSlice(line[0], 1, 1));

        char * temp = quokka_compile_line_tokens(templine, num, templen, 1);

        mstrcat(&bytecode, strndup(temp, strlen(temp)));

        free(temp);

        mstrcat(&bytecode, "MAKE_LIST");
        mstrcat(&bytecode, SEPARATOR);
        if (templen)
            mstrcat(&bytecode, intToStr(stringCountUntil(templine, ",", templen) + 1));
        else
            mstrcat(&bytecode, "0");
        mstrcat(&bytecode, INSTRUCTION_END);

        free(templine);
    }
    else
    {
        if (isInline)
        {
            error("invalid syntax", num);
            return bytecode;
        }

        // Set new line
        mstrcattrip(&bytecode, intToStr(current_line + 1), INSTRUCTION_END);

        int lastwascomma = 0;

        for (int p = 0; p < len; p++)
        {
            if (!strcmp(line[p], ","))
            {
                lastwascomma = 1;
                continue;
            }

            if (!lastwascomma && p > 0)
            {
                error("invalid syntax", num);
                break;
            }

            if (!strlen(line[p]))
                break;

            char * temp = quokka_compile_line(line[p], num, 1, 1);

            mstrcat(&bytecode, strndup(temp, strlen(temp)));

            free(temp);

            lastwascomma = 0;
        }
    }

    return bytecode;
}

char * quokka_compile_tokens(char ** tokens, int isInline)
{
    // This will eventually need to be fixed and made dynamic
    char * compiled = malloc(1);
    strcpy(compiled, "");

    file_line_count = arrsize(tokens);
    current_line = 0;

    while (current_line < file_line_count)
    {
        char * t = tokens[current_line];

        if (t == NULL)
            continue;

        char * ins = quokka_compile_line(t, current_line, -1, isInline);

        mstrcat(&compiled, strndup(ins, strlen(ins)));
        free(ins);

        current_line++;
    }

    free(tokens);

    return compiled;
}

char * quokka_compile_raw(char * rawtext, int maxtokensize, int isInline)
{
    if (maxtokensize == -1)
        maxtokensize = 2048;
    file_tokens = malloc(maxtokensize + 1);
    ntokenise(file_tokens, rawtext, "\n");

    char * res = quokka_compile_tokens(file_tokens, isInline);

    return res;
}

char * quokka_compile_fname(char * filename)
{
    char * buffer = readfile(filename);

    if (!buffer)
        return 0;

    char * res = quokka_compile_raw(buffer, -1, 0);

    return res;
}

char ** quokka_line_tok(char * line)
{
    line = cpstrip(line);

    char * tokenstr = malloc(1);
    strcpy(tokenstr, "");

    // Bools
    int sq = 0; // Single-Quote = false
    int dq = 0; // Double-Quote = false
    int bt = 0; // Backtick = false

    // Ints
    int rb = 0; // Regular-bracket = 0
    int sb = 0; // Square-bracket = 0
    int cb = 0; // Curly-bracket = 0

    // Used to identify a `//` comment
    int comment = 0; // 1 if `/` found, else 0    

    // Chars
    char q;
    char p = '\0';
    char t = 0;

    // Strings
    char * separator = "\n";

    // Set initial char type
    if (isalnum(line[0]))
        p = 'A';
    else if (isdigit(line[0]))
        p = 'D';
    else if (ispunct(line[0]))
        p = 'S';
    else if (isspace(line[0]))
        p = 'W';

    int len = strlen(line);
    for (int i = 0; i < len; i++)
    {
        q = p;
        char c = line[i];
        if (isalpha(c))
            p = 'A'; // Alphabet
        else if (isdigit(c))
            p = 'D'; // Digit
        else if (ispunct(c))
            p = 'S'; // Symbol
        else if (isspace(c))
            p = 'W'; // Whitespace
        if (((q != p && p != 'W') || p == 'S') && !(
            sq || dq || bt || rb > 0 || sb > 0 || cb > 0
        ) && !(
            // Join together operators: -= += *= /= == >= <=
            (t == '-' || t == '+' || t == '*' || t == '/' || t == '=' || t == '>' || t == '>') && c == '='
        // ) && !(
        //     q == 'A' && c == '.' // Join together names like `word.upper` (second part is below)
        ) && !(
            p == 'A' && t == '.' // Second part to the line above.
        ) && !(
            t == '_' && (p == 'A' || c == '.') // Join together names like `string_one` (second part is below)
        ) && !(
            (q == 'A' || t == '.') && c == '_' // Second part to the line above.
        ) && !(
            t == '_' && c == '_' // Join together double underscores `__`
        ) && !(
            q == 'A' && p == 'D' // Join alphabetical and numerical characters.
        ) && !(
            p == 'A' && q == 'D' // Second part to the line above.
        ) && !(
            q == 'D' && (c == 'l' || c == 'L') // Join integers with `l` and `L` for long numbers
        ) && !(
            t == '.' && c == '.' // Join together all `.` tokens.
        ) && !(
            t == '*' && c == '*' // Join together all `*` tokens.
        ))
        {
            tokenstr = realloc(tokenstr, strlen(tokenstr) + strlen(separator) + 1);
            strncat(tokenstr, separator, strlen(separator));
        }
        if (c == '\'' && !(
            dq || rb > 0 || sb > 0 || cb > 0))
            sq = !sq;
        else if (c == '"' && !(
            sq || bt || rb > 0 || sb > 0 || cb > 0))
            dq = !dq;
        else if (c == '`' && !(
            sq || dq || rb > 0 || sb > 0 || cb > 0))
            bt = !bt;
        else if (c == '(' && !(
            sq || dq || bt || sb > 0 || cb > 0))
            rb++;
        else if (c == ')' && !(
            sq || dq || bt || sb > 0 || cb > 0))
            rb--;
        else if (c == '[' && !(
            sq || dq || bt || rb > 0 || cb > 0))
            sb++;
        else if (c == ']' && !(
            sq || dq || bt || rb > 0 || cb > 0))
            sb--;
        else if (c == '/' && !(
            sq || dq || bt))
        {
            comment++;
            if (comment >= 2)
            {
                tokenstr[strlen(tokenstr) - 2] = '\0';
                break;
            }
        }
        if (comment <= 1)
        {
            if (comment && c != '/')
                comment = 0;
            tokenstr = realloc(tokenstr, strlen(tokenstr) + 2);
            strncat(tokenstr, &c, 1);
        }
        t = c;
    }

    char ** output = malloc(sizeof(char *));
    output[0] = "";

    int i = 0;
    output[0] = cpstrip(strtok(tokenstr, separator));

    while (output[i] != NULL)
    {
        i++;
        output = realloc(output, (i + 1) * sizeof(char *));
        output[i] = cpstrip(strtok(NULL, separator));
    }

    output = realloc(output, (i + 2) * sizeof(char *));
    output[i + 1] = NULL;

    return output;
}
