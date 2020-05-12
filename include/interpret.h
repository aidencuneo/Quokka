void interp_init();
void quokka_interpret_line(char * linetext);
void quokka_interpret_line_tokens(char ** line);
void quokka_interpret_tokens(char ** tokens);
void quokka_interpret(char * bytecode);

// Important stuff
Varlist vars;

Object * methods;
int method_count;

Object * stack;
int stack_size;

// Bytecode file stuff
char ** bc_tokens;
int bc_line;
int bc_line_count;

// Multi-use pointers
int oneArgc;
int twoArgc;

int truePtr;
int falsePtr;

void resetStack()
{
    stack = malloc(sizeof(Object));
    stack_size = 0;
}

void interp_init()
{
    resetStack();

    oneArgc = 1;
    twoArgc = 2;

    truePtr = 1;
    falsePtr = 0;

    // No argument restraints
    Object printFunction = emptyObject("function");
    printFunction = addObjectValue(printFunction, "__call__", &q_function_print);
    // printFunction = addObjectValue(printFunction, "__call__argc", &oneArgc);
    addVar("print", printFunction);

    // No argument restraints
    Object printlnFunction = emptyObject("function");
    printlnFunction = addObjectValue(printlnFunction, "__call__", &q_function_println);
    // printlnFunction = addObjectValue(printlnFunction, "__call__argc", &oneArgc);
    addVar("println", printlnFunction);

    Object inputFunction = emptyObject("function");
    inputFunction = addObjectValue(inputFunction, "__call__", &q_function_input);
    inputFunction = addObjectValue(inputFunction, "__call__argmin", &falsePtr);
    inputFunction = addObjectValue(inputFunction, "__call__argmax", &oneArgc);
    addVar("input", inputFunction);

    Object stringFunction = emptyObject("function");
    stringFunction = addObjectValue(stringFunction, "__call__", &q_function_string);
    stringFunction = addObjectValue(stringFunction, "__call__argmin", &oneArgc);
    stringFunction = addObjectValue(stringFunction, "__call__argmax", &oneArgc);
    addVar("string", stringFunction);

    Object intFunction = emptyObject("function");
    intFunction = addObjectValue(intFunction, "__call__", &q_function_int);
    intFunction = addObjectValue(intFunction, "__call__argcmin", &oneArgc);
    intFunction = addObjectValue(intFunction, "__call__argcmax", &oneArgc);
    addVar("int", intFunction);

    Object boolFunction = emptyObject("function");
    boolFunction = addObjectValue(boolFunction, "__call__", &q_function_bool);
    boolFunction = addObjectValue(boolFunction, "__call__argmin", &oneArgc);
    boolFunction = addObjectValue(boolFunction, "__call__argmax", &oneArgc);
    addVar("bool", boolFunction);

    Object typeFunction = emptyObject("function");
    typeFunction = addObjectValue(typeFunction, "__call__", &q_function_type);
    typeFunction = addObjectValue(typeFunction, "__call__argmin", &oneArgc);
    typeFunction = addObjectValue(typeFunction, "__call__argmax", &oneArgc);
    addVar("type", typeFunction);
}

Object emptyObject(char * name)
{
    Object self;

    self.name = name;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));
    self.value_count = 0;

    return self;
}

Object makeObject(char * name, void * value)
{
    Object self;

    self.name = name;

    self.names = malloc(sizeof(char *));
    self.values = malloc(sizeof(void *));
    self.value_count = 0;

    self = addObjectValue(self, "value", value);

    return self;
}

Object addObjectValue(Object obj, char * name, void * value)
{
    obj.names = realloc(obj.names, (obj.value_count + 1) * sizeof(char *));
    obj.values = realloc(obj.values, (obj.value_count + 1) * sizeof(void *));

    obj.names[obj.value_count] = name;
    obj.values[obj.value_count] = value;

    obj.value_count++;

    return obj;
}

int objectHasAttr(Object obj, char * name)
{
    for (int i = 0; i < obj.value_count; i++)
        if (!strcmp(obj.names[i], name))
            return 1;

    return 0;
}

void * objectGetAttr(Object obj, char * name)
{
    for (int i = 0; i < obj.value_count; i++)
        if (!strcmp(obj.names[i], name))
            return obj.values[i];
}

void pushTop(Object obj)
{
    stack = realloc(stack, (stack_size + 1) * sizeof(Object));
    stack[stack_size] = obj;
    stack_size++;
}

Object popTop()
{
    stack = realloc(stack, (stack_size + 1) * sizeof(Object));
    stack_size--;

    return stack[stack_size];
}

void addVar(char * name, Object obj)
{
    int check = getVarIndex(name);
    if (check != -1)
    {
        vars.values[check] = obj;
        return;
    }

    vars.names = realloc(vars.names, (vars.count + 1) * sizeof(char *));
    vars.values = realloc(vars.values, (vars.count + 1) * sizeof(Object));

    vars.names[vars.count] = name;
    vars.values[vars.count] = obj;

    vars.count++;
}

Object getVar(char * name)
{
    for (int i = 0; i < vars.count; i++)
        if (!strcmp(vars.names[i], name))
            return vars.values[i];

    char * err = malloc(1 + strlen(name) + 16 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined");
    error(err, line_num);
}

int getVarIndex(char * name)
{
    for (int i = 0; i < vars.count; i++)
        if (!strcmp(vars.names[i], name))
            return i;

    return -1;
}

// void addMethod(Object method)
// {
//     methods = realloc(methods, (method_count + 1) * sizeof(Object));
//     methods[method_count] = method;
//     method_count++;
// }

//
/// datatypes
//

#include "datatypes/int.h"
#include "datatypes/string.h"
#include "datatypes/list.h"
#include "datatypes/null.h"

// Object makeMethod(Object (*func)(Object * argv), int * argc)
// {
//     Object self;

//     self = makeObject("method", argc);
//     self = addObjectValue(self, "__call__", &func);

//     return self;
// }

void quokka_interpret_line(char * linetext)
{
    char ** line = quokka_line_tok(linetext);
    quokka_interpret_line_tokens(line);

    free(line);
}

void quokka_interpret_line_tokens(char ** line)
{
    if (!strlen(line[0]))
        return;

    if (verbose) println(line[0]);

    if (!strcmp(line[0], "LOAD_STRING"))
    {
        Object item = makeString(makeLiteralString(line[1]));

        pushTop(item);
    }
    else if (!strcmp(line[0], "LOAD_INT"))
    {
        Object item = makeInteger(makeIntPtrFromStr(line[1]));

        pushTop(item);
    }
    else if (!strcmp(line[0], "LOAD_NAME"))
    {
        pushTop(getVar(line[1]));
    }
    else if (!strcmp(line[0], "MAKE_LIST"))
    {
        int lstsize = strtol(line[1], NULL, 10);

        Object * value = malloc(lstsize * sizeof(Object));

        for (int i = 0; i < lstsize; i++)
            value[i] = popTop();

        pushTop(makeList(lstsize, value));
    }
    else if (!strcmp(line[0], "UNARY_ADD"))
    {
        Object first = popTop();

        if (!objectHasAttr(first, "__pos__"))
        {
            char * err = malloc(6 + strlen(first.name) + 38 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for unary '+'");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__pos__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __pos__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__pos__argc"))[0];
        if (funcargc != 1)
            error("__pos__ function requires an invalid amount of arguments, should be 1", line_num);

        Object arglist[1];
        arglist[0] = first;

        pushTop(((standard_func_def)objectGetAttr(first, "__pos__"))(1, arglist));
    }
    else if (!strcmp(line[0], "UNARY_SUB"))
    {
        Object first = popTop();

        if (!objectHasAttr(first, "__neg__"))
        {
            char * err = malloc(6 + strlen(first.name) + 38 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for unary '-'");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__neg__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __neg__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__neg__argc"))[0];
        if (funcargc != 1)
            error("__neg__ function requires an invalid amount of arguments, should be 1", line_num);

        Object arglist[1];
        arglist[0] = first;

        pushTop(((standard_func_def)objectGetAttr(first, "__neg__"))(1, arglist));
    }
    else if (!strcmp(line[0], "BINARY_ADD"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__add__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for addition");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__add__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __add__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__add__argc"))[0];
        if (funcargc != 2)
            error("__add__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__add__"))(2, arglist));
    }
    else if (!strcmp(line[0], "BINARY_SUB"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__sub__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for subtraction");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__sub__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __sub__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__sub__argc"))[0];
        if (funcargc != 2)
            error("__sub__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__sub__"))(2, arglist));
    }
    else if (!strcmp(line[0], "BINARY_MUL"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__mul__"))
        {
            char * err = malloc(6 + strlen(first.name) + 43 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for multiplication");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__mul__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __mul__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__mul__argc"))[0];
        if (funcargc != 2)
            error("__mul__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__mul__"))(2, arglist));
    }
    else if (!strcmp(line[0], "BINARY_DIV"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__div__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for division");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__div__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __div__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__div__argc"))[0];
        if (funcargc != 2)
            error("__div__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__div__"))(2, arglist));
    }
    else if (!strcmp(line[0], "BINARY_POW"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__pow__"))
        {
            char * err = malloc(6 + strlen(first.name) + 37 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for indices");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__pow__argc"))
        {
            char * err = malloc(28 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __pow__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__pow__argc"))[0];
        if (funcargc != 2)
            error("__pow__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__pow__"))(2, arglist));
    }
    else if (!strcmp(line[0], "CMP_EQ"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__eq__"))
        {
            char * err = malloc(6 + strlen(first.name) + 53 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for equality (==) comparison");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__eq__argc"))
        {
            char * err = malloc(27 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __eq__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__eq__argc"))[0];
        if (funcargc != 2)
            error("__eq__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__eq__"))(2, arglist));
    }
    else if (!strcmp(line[0], "CMP_LT"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__lt__"))
        {
            char * err = malloc(6 + strlen(first.name) + 53 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for less than (<) comparison");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__lt__argc"))
        {
            char * err = malloc(27 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __lt__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__lt__argc"))[0];
        if (funcargc != 2)
            error("__lt__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__lt__"))(2, arglist));
    }
    else if (!strcmp(line[0], "CMP_GT"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__gt__"))
        {
            char * err = malloc(6 + strlen(first.name) + 56 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for greater than (>) comparison");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__gt__argc"))
        {
            char * err = malloc(27 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __gt__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__gt__argc"))[0];
        if (funcargc != 2)
            error("__gt__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__gt__"))(2, arglist));
    }
    else if (!strcmp(line[0], "CMP_LE"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__le__"))
        {
            char * err = malloc(6 + strlen(first.name) + 66 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for less than or equal to (<=) comparison");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__le__argc"))
        {
            char * err = malloc(27 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __le__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__le__argc"))[0];
        if (funcargc != 2)
            error("__le__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__le__"))(2, arglist));
    }
    else if (!strcmp(line[0], "CMP_GE"))
    {
        Object secnd = popTop();
        Object first = popTop();

        if (!objectHasAttr(first, "__ge__"))
        {
            char * err = malloc(6 + strlen(first.name) + 69 + 1);
            strcpy(err, "type '");
            strcat(err, first.name);
            strcat(err, "' does not have a method for greater than or equal to (>=) comparison");
            error(err, line_num);
        }

        if (!objectHasAttr(first, "__ge__argc"))
        {
            char * err = malloc(27 + strlen(first.name) + 56 + 1);
            strcpy(err, "the __ge__ method of type '");
            strcat(err, first.name);
            strcat(err, "' is missing an argument limit, this should never happen");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(first, "__ge__argc"))[0];
        if (funcargc != 2)
            error("__ge__ function requires an invalid amount of arguments, should be 2", line_num);

        Object arglist[2];
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__ge__"))(2, arglist));
    }
    else if (!strcmp(line[0], "JUMP_TO"))
    {
        for (int i = bc_line; i < bc_line_count; i++)
        {
            if (!strcmp(bc_tokens[i], line[1]))
            {
                bc_line = i;
                break;
            }
        }
    }
    else if (!strcmp(line[0], "JUMP_BACK"))
    {
        for (int i = bc_line; i >= 0; i--)
        {
            if (!strcmp(bc_tokens[i], line[1]))
            {
                bc_line = i;
                break;
            }
        }
    }
    else if (!strcmp(line[0], "JUMP_IF_FALSE"))
    {
        Object obj = popTop();

        Object * arglist = malloc(sizeof(Object));
        arglist[0] = obj;

        if (!objectHasAttr(obj, "__bool__"))
        {
            char * err = malloc(6 + strlen(obj.name) + 34 + 1);
            strcpy(err, "type '");
            strcat(err, obj.name);
            strcat(err, "' can not be converted into a bool");
            error(err, line_num);
        }

        Object conditionobj = ((standard_func_def)objectGetAttr(obj, "__bool__"))(1, arglist);
        int condition = ((int *)objectGetAttr(conditionobj, "value"))[0];

        if (condition)
            return;

        // If false, skip the if statement

        for (int i = bc_line; i < bc_line_count; i++)
        {
            if (!strcmp(bc_tokens[i], line[1]))
            {
                bc_line = i;
                break;
            }
        }
    }
    else if (!strcmp(line[0], "STORE_NAME"))
    {
        addVar(line[1], popTop());
    }
    else if (!strcmp(line[0], "CALL_FUNCTION"))
    {
        int argcount = strtol(line[1], NULL, 10);
        Object * arglist = malloc(argcount * sizeof(Object));

        for (int i = 0; i < argcount; i++)
            arglist[argcount - i - 1] = popTop();

        Object func = popTop();

        if (!objectHasAttr(func, "__call__"))
            error("not a callable type", line_num);

        int funcmin = 0;
        int funcmax = -1;

        if (objectHasAttr(func, "__call__argc"))
        {
            funcmin = ((int *)objectGetAttr(func, "__call__argc"))[0];
            funcmax = funcmin;
        }

        if (objectHasAttr(func, "__call__argmin"))
            funcmin = ((int *)objectGetAttr(func, "__call__argmin"))[0];

        if (objectHasAttr(func, "__call__argmax"))
            funcmax = ((int *)objectGetAttr(func, "__call__argmax"))[0];

        if (argcount > funcmax && funcmax != -1)
            error("function received too many arguments", line_num);
        if (argcount < funcmin)
        {
            int diff = funcmin - argcount;
            char * diffstr = malloc(11);
            sprintf(diffstr, "%d", diff);

            char * err;
            if (diff != 1)
                err = malloc(27 + strlen(diffstr) + 23 + 1 + 1);
            else err = malloc(27 + strlen(diffstr) + 23 + 1);

            strcpy(err, "function requires at least ");
            strcat(err, strndup(diffstr, strlen(diffstr)));
            strcat(err, " more argument");
            if (diff != 1)
                strcat(err, "s");

            free(diffstr);
            error(err, line_num);
        }

        pushTop(((standard_func_def)objectGetAttr(func, "__call__"))(argcount, arglist));
    }
    else if (!strcmp(line[0], "GET_INDEX"))
    {
        // If 'GET_INDEX *' then use __copy__

        if (!strcmp(line[1], "*"))
        {
            Object obj = popTop();

            if (!objectHasAttr(obj, "__copy__"))
            {
                char * err = malloc(6 + strlen(obj.name) + 36 + 1);
                strcpy(err, "type '");
                strcat(err, obj.name);
                strcat(err, "' does not have a method for copying");
                error(err, line_num);
            }

            Object * arglist = malloc(sizeof(Object));
            arglist[0] = obj;

            pushTop(((standard_func_def)objectGetAttr(obj, "__copy__"))(1, arglist));

            return;
        }

        // If 'GET_INDEX 1' then get index like normal

        Object ind = popTop();
        Object obj = popTop();

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = obj;
        arglist[1] = ind;

        if (!objectHasAttr(obj, "__index__"))
        {
            char * err = malloc(6 + strlen(obj.name) + 45 + 1);
            strcpy(err, "type '");
            strcat(err, obj.name);
            strcat(err, "' does not have a method for index retrieving");
            error(err, line_num);
        }

        int funcargc = ((int *)objectGetAttr(obj, "__index__argc"))[0];
        if (funcargc != 2)
            error("__index__ function requires an invalid amount of arguments, should be 2", line_num);

        pushTop(((standard_func_def)objectGetAttr(obj, "__index__"))(2, arglist));
    }
}

void quokka_interpret_tokens(char ** tokens)
{
    bc_line_count = arrsize(tokens);

    bc_line = 0;
    while (bc_line < bc_line_count)
    {
        char * t = tokens[bc_line];

        if (t == NULL)
            continue;

        if (isinteger(t))
        {
            line_num = strtol(t, NULL, 10) - 1;
            resetStack();

            bc_line++;
            continue;
        }

        quokka_interpret_line(t);

        bc_line++;
    }

    free(tokens);
}

void quokka_interpret(char * bytecode)
{
    // 8192 is the line limit for a bytecode file
    bc_tokens = malloc(8192 * sizeof(char *));
    ntokenise(bc_tokens, bytecode, "\n");

    quokka_interpret_tokens(bc_tokens);
}
