// Important stuff
Varlist globals;
Varlist locals;

Object * methods;
int method_count;

Object * stack;
int stack_size;

Object * consts;
int constcount;

Object * ret_stack;
int ret_stack_size;
int can_return;

// Trash
void ** trash;
int trashsize;

// Bytecode file stuff
char ** bc_tokens;

// Multi-use pointers
int oneArgc;
int twoArgc;

int truePtr;
int falsePtr;

// Stack
void freeStack()
{
    for (int i = 0; i < stack_size; i++)
        freeObject(stack[i]);

    free(stack);
}

void resetStack()
{
    freeStack();

    stack = malloc(sizeof(Object));
    stack_size = 0;
}

// Return Stack
void resetRetStack()
{
    // for (int i = 0; i < ret_stack_size; i++)
    //     freeObject(ret_stack[i]);

    free(ret_stack);
    ret_stack = malloc(sizeof(Object));
    ret_stack_size = 0;
}

void freeRetStack()
{
    free(ret_stack);
}

// Var cleaning
void freeGlobals()
{
    for (int i = 0; i < globals.count; i++)
        freeObject(globals.values[i]);

    free(globals.names);
    free(globals.values);
}

void freeLocals()
{
    for (int i = 0; i < locals.count; i++)
        freeObject(locals.values[i]);

    free(locals.names);
    free(locals.values);
}

void freeVars()
{
    freeGlobals();
    freeLocals();
}

// Constants
void freeConsts()
{
    clearConsts();
    free(consts);
}

// Free
void emptyTrash()
{
    for (int i = 0; i < trashsize; i++)
        free(trash[i]);

    free(trash);
    trashsize = 0;
}

// Cleanup (ONLY called on a sudden exit)
void cleanupAll()
{
    free(full_file_name);
    free(full_dir_name);
    free(main_bytecode);
    free(bc_tokens);

    freeVars();
    freeStack();
    freeRetStack();
    emptyTrash();
}

// Init
void interp_init()
{
    resetStack();
    resetRetStack();

    oneArgc = 1;
    twoArgc = 2;

    truePtr = 1;
    falsePtr = 0;

    /*
    Global Variables
    */

    addGVar("true", makeInt(&truePtr));
    addGVar("false", makeInt(&falsePtr));

    /*
    No argument restraints
    */

    Object printFunction = emptyObject("bfunction");
    printFunction = addObjectValue(printFunction, "__call__", &q_function_print);
    addGVar("print", printFunction);

    Object printlnFunction = emptyObject("bfunction");
    printlnFunction = addObjectValue(printlnFunction, "__call__", &q_function_println);
    addGVar("println", printlnFunction);

    /*
    Argc : 0
    */

    Object exitFunction = emptyObject("bfunction");
    exitFunction = addObjectValue(exitFunction, "__call__", &q_function_exit);
    exitFunction = addObjectValue(exitFunction, "__call__argmin", &falsePtr);
    exitFunction = addObjectValue(exitFunction, "__call__argmax", &falsePtr);
    addGVar("exit", exitFunction);

    /*
    Argc : 1
    */

    Object stringFunction = emptyObject("bfunction");
    stringFunction = addObjectValue(stringFunction, "__call__", &q_function_string);
    stringFunction = addObjectValue(stringFunction, "__call__argmin", &oneArgc);
    stringFunction = addObjectValue(stringFunction, "__call__argmax", &oneArgc);
    addGVar("string", stringFunction);

    Object intFunction = emptyObject("bfunction");
    intFunction = addObjectValue(intFunction, "__call__", &q_function_int);
    intFunction = addObjectValue(intFunction, "__call__argcmin", &oneArgc);
    intFunction = addObjectValue(intFunction, "__call__argcmax", &oneArgc);
    addGVar("int", intFunction);

    Object longFunction = emptyObject("bfunction");
    longFunction = addObjectValue(longFunction, "__call__", &q_function_long);
    longFunction = addObjectValue(longFunction, "__call__argcmin", &oneArgc);
    longFunction = addObjectValue(longFunction, "__call__argcmax", &oneArgc);
    addGVar("long", longFunction);

    Object boolFunction = emptyObject("bfunction");
    boolFunction = addObjectValue(boolFunction, "__call__", &q_function_bool);
    boolFunction = addObjectValue(boolFunction, "__call__argmin", &oneArgc);
    boolFunction = addObjectValue(boolFunction, "__call__argmax", &oneArgc);
    addGVar("bool", boolFunction);

    Object typeFunction = emptyObject("bfunction");
    typeFunction = addObjectValue(typeFunction, "__call__", &q_function_type);
    typeFunction = addObjectValue(typeFunction, "__call__argmin", &oneArgc);
    typeFunction = addObjectValue(typeFunction, "__call__argmax", &oneArgc);
    addGVar("type", typeFunction);

    Object dispFunction = emptyObject("bfunction");
    dispFunction = addObjectValue(dispFunction, "__call__", &q_function_display);
    dispFunction = addObjectValue(dispFunction, "__call__argmin", &oneArgc);
    dispFunction = addObjectValue(dispFunction, "__call__argmax", &oneArgc);
    addGVar("disp", dispFunction);

    Object lenFunction = emptyObject("bfunction");
    lenFunction = addObjectValue(lenFunction, "__call__", &q_function_len);
    lenFunction = addObjectValue(lenFunction, "__call__argmin", &oneArgc);
    lenFunction = addObjectValue(lenFunction, "__call__argmax", &oneArgc);
    addGVar("len", lenFunction);

    Object execFunction = emptyObject("bfunction");
    execFunction = addObjectValue(execFunction, "__call__", &q_function_exec);
    execFunction = addObjectValue(execFunction, "__call__argmin", &oneArgc);
    execFunction = addObjectValue(execFunction, "__call__argmax", &oneArgc);
    addGVar("exec", execFunction);

    Object sizeofFunction = emptyObject("bfunction");
    sizeofFunction = addObjectValue(sizeofFunction, "__call__", &q_function_sizeof);
    sizeofFunction = addObjectValue(sizeofFunction, "__call__argmin", &oneArgc);
    sizeofFunction = addObjectValue(sizeofFunction, "__call__argmax", &oneArgc);
    addGVar("sizeof", sizeofFunction);

    /*
    The rest
    */

    Object inputFunction = emptyObject("bfunction");
    inputFunction = addObjectValue(inputFunction, "__call__", &q_function_input);
    inputFunction = addObjectValue(inputFunction, "__call__argmin", &falsePtr);
    inputFunction = addObjectValue(inputFunction, "__call__argmax", &oneArgc);
    addGVar("input", inputFunction);
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

// Duplicate an Object
Object objectCopy(Object orig)
{
    Object self = emptyObject(orig.name);

    for (int i = 0; i < orig.value_count; i++)
    {
        char * name = malloc(strlen(orig.names[i]) + 1);
        strcpy(name, orig.names[i]);
        pushTrash(name);

        void ** newptr = malloc(sizeof(void *));
        *newptr = orig.values[i];
        pushTrash(newptr);

        self = addObjectValue(self, name, *newptr);
    }

    // If object is iterable, duplicate all objects inside it
    // if (!strcmp(self.name, "list"))
    // {
    //     int length = ((int *)objectGetAttr(self, "length"))[0];
    //     Object * lst = objectGetAttr(self, "value");
    //     for (int i = 0; i < length; i++)
    //     {
    //         // lst[i] = makeNull();//objectCopy(lst[i]);
    //     }
    // }

    return self;
}

// Summarise an Object (for debugging - uncomment to use it)
void objectSummary(Object obj)
{
    print("Object of type <");
    print(obj.name);
    print(">, value_count = ");
    println(obj.value_count);
    println("-- ATTRIBUTES --");

    for (int i = 0; i < obj.value_count; i++)
    {
        print(": ");
        println(obj.names[i]);
    }
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

    return NULL;
}

// Free an object and all objects within (if object is iterable)
void freeObjectR(Object obj)
{
    // If obj is iterable, free all items within it
    if (!strcmp(obj.name, "list"))
    {
        int length = ((int *)objectGetAttr(obj, "length"))[0];
        Object * lst = objectGetAttr(obj, "value");
        for (int i = 0; i < length; i++)
        {
            freeObjectR(lst[i]);
        }
    }

    free(obj.names);
    free(obj.values);
}

// Free an object, no recursion for lists
void freeObject(Object obj)
{
    free(obj.names);
    free(obj.values);
}

/*

Stack

*/

// Push to top of stack normally
void pushTop(Object obj)
{
    stack = realloc(stack, (stack_size + 1) * sizeof(Object));
    stack[stack_size] = obj;
    stack_size++;
}

// Push to top of stack using const.
// If you don't need this Object to be in consts, just use
// pushTop, it's faster in the long run unless you need
// `obj` to be in consts and a duplicate to be pushed into
// the stack.
void pushTopC(Object obj)
{
    pushConst(obj);
    pushTop(objectCopy(consts[constcount - 1]));
}

// Push to bottom of stack manually (using your own Object)
void pushBottomM(Object obj)
{
    stack = realloc(stack, (stack_size + 1) * sizeof(Object));

    for (int i = stack_size; i > 0; i--)
    {
        stack[i] = stack[i - 1];
    }
    stack[0] = obj;

    stack_size++;
}

// Push to bottom of stack using const
void pushBottom(Object obj)
{
    pushConst(obj);
    pushBottomM(objectCopy(consts[constcount - 1]));
}

// Pop top of stack
Object popTop()
{
    stack = realloc(stack, stack_size * sizeof(Object));
    stack_size--;

    return stack[stack_size];
}

/*

Return Stack

*/

void pushRetTop(Object obj)
{
    ret_stack = realloc(ret_stack, (ret_stack_size + 1) * sizeof(Object));
    ret_stack[ret_stack_size] = obj;
    ret_stack_size++;
}

Object popRetTop()
{
    ret_stack = realloc(ret_stack, (ret_stack_size + 1) * sizeof(Object));
    ret_stack_size--;

    return ret_stack[ret_stack_size];
}

// Constants
void pushConst(Object obj)
{
    consts = realloc(consts, (constcount + 1) * sizeof(Object));
    consts[constcount] = obj;
    constcount++;
}

// Clear constants after a line ending or after
// an expression has been evaluated
void clearConsts()
{
    for (int i = 0; i < constcount; i++)
    {
        freeObjectR(consts[i]);
    }

    consts = realloc(consts, sizeof(Object));
    constcount = 0;
}

// DO NOT pass an un-malloc'd pointer to this function
void pushTrash(void * ptr)
{
    trash = realloc(trash, (trashsize + 1) * sizeof(void *));
    trash[trashsize] = ptr;
    trashsize++;
}

// Var stuff
void addGVar(char * name, Object obj)
{
    int check = getGVarIndex(name);
    if (check != -1)
    {
        // Free the old object non-recursively to prevent lists from losing their objects
        freeObject(globals.values[check]);

        globals.values[check] = obj;
        return;
    }

    globals.names = realloc(globals.names, (globals.count + 1) * sizeof(char *));
    globals.values = realloc(globals.values, (globals.count + 1) * sizeof(Object));

    globals.names[globals.count] = name;
    globals.values[globals.count] = obj;

    globals.count++;
}

// addVar assigns local variables by default.
// use addGVar to assign a global variable.
void addVar(char * name, Object obj)
{
    int check = getLVarIndex(name);
    if (check != -1)
    {
        // Free the old object non-recursively to prevent lists from losing their objects
        freeObject(locals.values[check]);

        locals.values[check] = obj;
        return;
    }

    locals.names = realloc(locals.names, (locals.count + 1) * sizeof(char *));
    locals.values = realloc(locals.values, (locals.count + 1) * sizeof(Object));

    locals.names[locals.count] = name;
    locals.values[locals.count] = obj;

    locals.count++;
}

Object getGVar(char * name)
{
    // Check globals
    for (int i = 0; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    char * err = malloc(1 + strlen(name) + 37 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined as a global variable");
    error(err, line_num);

    return makeNull();
}

Object getLVar(char * name)
{
    // Check locals
    for (int i = 0; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    char * err = malloc(1 + strlen(name) + 37 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined as a local variable");
    error(err, line_num);

    return makeNull();
}

Object getVar(char * name)
{
    // Check locals
    for (int i = 0; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    // Check globals
    for (int i = 0; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    char * err = malloc(1 + strlen(name) + 16 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined");
    error(err, line_num);

    return makeNull();
}

int getGVarIndex(char * name)
{
    // Check globals
    for (int i = 0; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return i;

    return -1;
}

int getLVarIndex(char * name)
{
    // Check locals
    for (int i = 0; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return i;

    return -1;
}

int getVarIndex(char * name)
{
    // Check locals
    for (int i = 0; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return i;

    // Check globals
    for (int i = 0; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return i;

    return -1;
}

void delGVarIndex(int index)
{
    freeObject(globals.values[index]);

    for (int i = index; i < locals.count; i++)
    {
        globals.names[i] = globals.names[i + 1];
        globals.values[i] = globals.values[i + 1];
    }

    globals.count--;

    globals.names = realloc(globals.names, (globals.count + 1) * sizeof(char *));
    globals.values = realloc(globals.values, (globals.count + 1) * sizeof(Object));
}

void delLVarIndex(int index)
{
    freeObject(locals.values[index]);

    for (int i = index; i < locals.count; i++)
    {
        locals.names[i] = locals.names[i + 1];
        locals.values[i] = locals.values[i + 1];
    }

    locals.count--;

    locals.names = realloc(locals.names, (locals.count + 1) * sizeof(char *));
    locals.values = realloc(locals.values, (locals.count + 1) * sizeof(Object));
}

Object * makeArglist(Object obj)
{
    Object * arglist = malloc(sizeof(Object));
    arglist[0] = obj;

    return arglist;
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
#include "datatypes/long.h"
#include "datatypes/string.h"
#include "datatypes/list.h"
#include "datatypes/null.h"
#include "datatypes/function.h"

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

/*

Temporary

*/

void CONSTS()
{
    print("CONSTANTS : ");
    for (int i = 0; i < constcount; i++)
    {
        Object * arglist = makeArglist(consts[i]);

        freeObjectR(q_function_print(1, arglist));
        free(arglist);

        print(", ");
    }
    println("");
}

void STACK()
{
    print("STACK : ");
    for (int i = 0; i < stack_size; i++)
    {
        Object * arglist = makeArglist(stack[i]);

        freeObjectR(q_function_print(1, arglist));
        free(arglist);

        print(", ");
    }
    println("");
}

void quokka_interpret_line_tokens(char ** line)
{
    if (!strlen(line[0]))
        return;

    if (verbose) STACK();
    if (verbose) println(line[0]);

    if (!strcmp(line[0], "LOAD_STRING"))
    {
        char * literal_str = makeLiteralString(line[1]);
        pushTrash(literal_str);

        pushTop(makeString(literal_str));
    }
    else if (!strcmp(line[0], "LOAD_INT"))
    {
        long long temp = strtoll(line[1], NULL, 10);
        if (temp > INT_MAX)
        {
            long long * llptr = makeLLPtrFromStr(line[1]);
            pushTrash(llptr);

            pushTop(makeLong(llptr));
        }
        else
        {
            int * intptr = makeIntPtrFromStr(line[1]);
            pushTrash(intptr);

            pushTop(makeInt(intptr));
        }
    }
    else if (!strcmp(line[0], "LOAD_LONG"))
    {
        long long * llptr = makeLLPtrFromStr(line[1]);
        pushTrash(llptr);

        pushTop(makeLong(llptr));
    }
    if (!strcmp(line[0], "LOAD_NULL"))
    {
        pushTop(makeNull());
    }
    else if (!strcmp(line[0], "LOAD_NAME"))
    {
        pushTop(objectCopy(getVar(line[1])));
    }
    else if (!strcmp(line[0], "STORE_NAME"))
    {
        addVar(line[1], popTop());
    }
    else if (!strcmp(line[0], "MAKE_LIST"))
    {
        int lstsize = strtol(line[1], NULL, 10);

        Object * value = malloc(lstsize * sizeof(Object));

        for (int i = 0; i < lstsize; i++)
        {
            // This line previously used objectCopy(popTop())
            value[i] = popTop();
        }

        pushTop(makeList(lstsize, value, 1));

        free(value);
    }
    else if (!strcmp(line[0], "DEFINE_FUNCTION"))
    {
        int argmin, argmax;

        if (!strcmp(line[2], "*"))
            argmin = 0;
        else
            argmin = strtol(line[2], NULL, 10);

        if (!strcmp(line[3], "*"))
            argmax = -1;
        else
            argmax = strtol(line[3], NULL, 10);

        char * replaced = strReplace(line[4], "\t", "\n");
        char * f_code = strSlice(replaced, 1, 1);

        free(replaced);

        // f_code points to the exact memory location of the new function's
        // bytecode, so f_code must be freed when the program exits.
        pushTrash(f_code);

        addGVar(line[1], makeFunction(&f_code, argmin, argmax));
    }
    else if (!strcmp(line[0], "DEL_VAR"))
    {
        int ind = getLVarIndex(line[1]);

        if (ind == -1)
        {
            char * err = malloc(1 + strlen(line[1]) + 16 + 1);
            strcpy(err, "'");
            strcat(err, line[1]);
            strcat(err, "' is not defined or can not be deleted");
            error(err, line_num);
        }

        delLVarIndex(ind);
    }
    else if (!strcmp(line[0], "RETURN"))
    {
        if (!can_return)
            error("can not return values from this scope", line_num);

        if (!stack_size)
            pushRetTop(makeNull());
        else
            pushRetTop(popTop());

        // Immediately return result and exit the function
        bc_line = bc_line_count;
    }
    // else if (!strcmp(line[0], "GET_ATTR"))
    // {
    //     Object obj = popTop();

    //     if (!objectHasAttr(obj, "__pos__"))
    //     {
    //         char * err = malloc(6 + strlen(obj.name) + 38 + 1);
    //         strcpy(err, "type '");
    //         strcat(err, obj.name);
    //         strcat(err, "' does not have a method for unary '+'");
    //         error(err, line_num);
    //     }

    //     freeObjectR(obj);
    // }
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

        Object * arglist = makeArglist(first);

        pushTop(((standard_func_def)objectGetAttr(first, "__pos__"))(1, arglist));

        free(arglist);
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

        Object * arglist = makeArglist(first);

        pushTop(((standard_func_def)objectGetAttr(first, "__neg__"))(1, arglist));

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__add__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__sub__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__mul__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__div__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__pow__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__eq__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_NEQ"))
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

        // Get result from __eq__
        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        Object condition = ((standard_func_def)objectGetAttr(first, "__eq__"))(2, arglist);

        // Convert to bool
        arglist = realloc(arglist, sizeof(Object));
        arglist[0] = condition;

        Object conditionbool = q_function_bool(1, arglist);

        // Flip it, then push it to stack
        int * intptr;

        if (!((int *)objectGetAttr(conditionbool, "value"))[0])
            intptr = &truePtr;
        else
            intptr = &falsePtr;

        pushTop(makeInt(intptr));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__lt__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__gt__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__le__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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

        Object * arglist = malloc(2 * sizeof(Object));
        arglist[0] = first;
        arglist[1] = secnd;

        pushTop(((standard_func_def)objectGetAttr(first, "__ge__"))(2, arglist));

        freeObject(first);
        freeObject(secnd);

        free(arglist);
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
    else if (!strcmp(line[0], "JUMP_IF_TRUE"))
    {
        Object obj = popTop();

        if (!objectHasAttr(obj, "__bool__"))
        {
            char * err = malloc(6 + strlen(obj.name) + 34 + 1);
            strcpy(err, "type '");
            strcat(err, obj.name);
            strcat(err, "' can not be converted into a bool");
            error(err, line_num);
        }

        Object * arglist = makeArglist(obj);
        Object conditionobj = q_function_bool(1, arglist);
        free(arglist);

        int condition = ((int *)objectGetAttr(conditionobj, "value"))[0]; //////////////////////////////////////////////////////////////

        freeObjectR(conditionobj);

        // If false, don't jump
        if (!condition)
            return;

        // If true, jump
        for (int i = bc_line; i < bc_line_count; i++)
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

        if (!objectHasAttr(obj, "__bool__"))
        {
            char * err = malloc(6 + strlen(obj.name) + 34 + 1);
            strcpy(err, "type '");
            strcat(err, obj.name);
            strcat(err, "' can not be converted into a bool");
            error(err, line_num);
        }

        Object * arglist = makeArglist(obj);
        Object conditionobj = q_function_bool(1, arglist);
        free(arglist);

        int condition = ((int *)objectGetAttr(conditionobj, "value"))[0]; ////////////////////////////////////////////////////////////////////////

        freeObjectR(conditionobj);

        // If true, don't jump
        if (condition)
            return;

        // If false, jump
        for (int i = bc_line; i < bc_line_count; i++)
        {
            if (!strcmp(bc_tokens[i], line[1]))
            {
                bc_line = i;
                break;
            }
        }
    }
    else if (!strcmp(line[0], "CALL"))
    {
        int argcount = strtol(line[1], NULL, 10);

        Object * arglist = malloc(argcount * sizeof(Object));

        int i;
        for (i = 0; i < argcount; i++)
        {
            // pushConst(popTop());
            // arglist[argcount - i - 1] = consts[constcount - 1]; // arglist[argcount - i - 1] = stack[stack_size - i - 1];
            arglist[argcount - i - 1] = popTop();
        }

        Object func = popTop(); // stack[stack_size - i - 1];

        if (!objectHasAttr(func, "__call__") &&
            strcmp(func.name, "bfunction") &&
            strcmp(func.name, "function"))
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
            char * diffstr = intToStr(diff);

            char * err;
            if (diff != 1)
                err = malloc(27 + strlen(diffstr) + 14 + 1 + 1);
            else err = malloc(27 + strlen(diffstr) + 14 + 1);

            strcpy(err, "function requires at least ");
            strcat(err, diffstr);
            strcat(err, " more argument");
            if (diff != 1)
                strcat(err, "s");

            free(diffstr);
            error(err, line_num);
        }

        if (!strcmp(func.name, "bfunction"))
        {
            pushTop(((standard_func_def)objectGetAttr(func, "__call__"))(argcount, arglist));

            // Free all arguments passed into this function
            for (int i = 0; i < argcount; i++)
                freeObject(arglist[i]);

            // Free the function itself
            freeObject(func);
        }
        else
        {
            arglist = realloc(arglist, (argcount + 1) * sizeof(Object));
            for (int i = argcount - 1; i >= 0; i--)
                arglist[i + 1] = arglist[i];
            arglist[0] = func;

            pushTop(((standard_func_def)objectGetAttr(func, "__call__"))(argcount, arglist));

            // Free all arguments passed into this function (function itself included)
            for (int i = 0; i < argcount + 1; i++)
                freeObject(arglist[i]);
        }

        free(arglist);
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

            Object * arglist = makeArglist(obj);

            pushTop(((standard_func_def)objectGetAttr(obj, "__copy__"))(1, arglist));

            free(arglist);

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

        freeObject(ind);
        freeObject(obj);

        free(arglist);
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
        {
            bc_line++;
            continue;
        }

        if (isinteger(t))
        {
            line_num = strtol(t, NULL, 10) - 1;

            // Comment the next two lines out if there's a segfault
            // if (!can_return)
            resetStack();

            // Clear constants after line ending
            clearConsts();

            bc_line++;
            continue;
        }

        quokka_interpret_line(t);

        bc_line++;
    }

    // if (!can_return)
    resetStack();

    // Clear constants at end of file (or bytecode segment)
    clearConsts();

    free(tokens);
}

void quokka_interpret(char * bytecode)
{
    bc_tokens = malloc(sizeof(char **));
    bc_tokens[0] = "";

    char * dupe = strndup(bytecode, strlen(bytecode));

    int i = 0;
    bc_tokens[0] = cpstrip(nstrtok(dupe, "\n"));

    while (bc_tokens[i] != NULL)
    {
        // Protects against empty parts of a line or string
        if (!strlen(bc_tokens[i]))
        {
            bc_tokens = realloc(bc_tokens, (i + 1) * sizeof(char *));
            bc_tokens[i] = cpstrip(nstrtok(NULL, "\n"));
            continue;
        }

        i++;
        bc_tokens = realloc(bc_tokens, (i + 1) * sizeof(char *));
        bc_tokens[i] = cpstrip(nstrtok(NULL, "\n"));
    }

    bc_tokens = realloc(bc_tokens, (i + 2) * sizeof(char *));
    bc_tokens[i + 1] = NULL;

    pushTrash(dupe);

    quokka_interpret_tokens(bc_tokens);
}
