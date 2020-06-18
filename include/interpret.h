// Important stuff
Varlist globals;
Varlist locals;

Object ** methods;
int method_count;

Object ** stack;
int stack_size;
int stack_alloc;
int stack_alloc_size = 10;

Object ** int_consts;
int int_const_count;

Object ** constants;
int constant_count;
int const_alloc;
int const_alloc_size = 20;

Object ** ret_stack;
int ret_stack_size;
int ret_stack_alloc;
int ret_stack_alloc_size = 5;

int can_return;

// Bytecode file stuff
char ** bc_tokens;

// Multi-use pointers
int oneArgc;
int twoArgc;
int threeArgc;

int truePtr;
int falsePtr;

// Stack
void freeStack()
{
    for (int i = 0; i < stack_size; i++)
        objUnref(stack[i]);

    free(stack);
}

void resetStack()
{
    freeStack();

    stack_alloc = stack_alloc_size;
    stack = malloc(stack_alloc * sizeof(Object *));
    stack_size = 0;
}

// Constants
void freeConsts()
{
    for (int i = 0; i < constant_count; i++)
        objDeref(constants[i]);

    free(constants);
}

void freeIntConsts()
{
    for (int i = 0; i < int_const_count; i++)
        objDeref(int_consts[i]);

    free(int_consts);
}

void initIntConsts()
{
    int_const_count = 16384; // 65536
    int_consts = malloc(int_const_count * sizeof(Object *));

    for (int i = 0; i < int_const_count; i++)
    {
        int * ptr = makeIntPtr(i);
        int_consts[i] = makeIntRaw(ptr, 1);
        int_consts[i]->refs++;
    }
}

// Return Stack
void freeRetStack()
{
    for (int i = 0; i < ret_stack_size; i++)
        objUnref(ret_stack[i]);

    free(ret_stack);
}

void resetRetStack()
{
    freeRetStack();

    ret_stack_alloc = ret_stack_alloc_size;
    ret_stack = malloc(ret_stack_alloc * sizeof(Object *));
    ret_stack_size = 0;
}

// Var cleaning
void freeGlobals()
{
    for (int i = 0; i < globals.count; i++)
        objUnref(globals.values[i]);

    free(globals.names);
    free(globals.values);
}

void freeLocals()
{
    for (int i = 0; i < locals.count; i++)
        objUnref(locals.values[i]);

    free(locals.names);
    free(locals.values);
}

void freeVars()
{
    freeGlobals();
    freeLocals();
}

// Cleanup (ONLY called on a sudden exit)
void cleanupAll()
{
    // free(bytecode_constants);
    // free(file_tokens);
    free(bc_tokens);

    free(scpstk);
    free(scps);
    free(scplines);

    free(full_file_name);
    free(full_dir_name);
    free(main_bytecode);

    freeStack();
    freeRetStack();
    freeVars();
    freeIntConsts();
    freeConsts();

    emptyTrash();
}

// Init
void interp_init()
{
    resetStack();
    resetRetStack();
    initIntConsts();

    oneArgc = 1;
    twoArgc = 2;
    threeArgc = 3;

    truePtr = 1;
    falsePtr = 0;

    /*
    Global Variables
    */

    addGVar("true", makeIntRaw(&truePtr, 0));
    addGVar("false", makeIntRaw(&falsePtr, 0));
    addGVar("PRINTSEP", makeString(" ", 0));

    /*
    No argument restraints
    */

    Object * printFunction = emptyObject("bfunction");
    printFunction = addObjectValue(printFunction, "__call__", &q_function_print);
    addGVar("print", printFunction);

    Object * printlnFunction = emptyObject("bfunction");
    printlnFunction = addObjectValue(printlnFunction, "__call__", &q_function_println);
    addGVar("println", printlnFunction);

    /*
    Argc : 0
    */

    Object * exitFunction = emptyObject("bfunction");
    exitFunction = addObjectValue(exitFunction, "__call__", &q_function_exit);
    exitFunction = addObjectValue(exitFunction, "__call__argc", &falsePtr);
    addGVar("exit", exitFunction);

    /*
    Argc : 1
    */

    Object * stringFunction = emptyObject("bfunction");
    stringFunction = addObjectValue(stringFunction, "__call__", &q_function_string);
    stringFunction = addObjectValue(stringFunction, "__call__argc", &oneArgc);
    addGVar("string", stringFunction);

    Object * intFunction = emptyObject("bfunction");
    intFunction = addObjectValue(intFunction, "__call__", &q_function_int);
    intFunction = addObjectValue(intFunction, "__call__argc", &oneArgc);
    addGVar("int", intFunction);

    Object * longFunction = emptyObject("bfunction");
    longFunction = addObjectValue(longFunction, "__call__", &q_function_long);
    longFunction = addObjectValue(longFunction, "__call__argc", &oneArgc);
    addGVar("long", longFunction);

    Object * boolFunction = emptyObject("bfunction");
    boolFunction = addObjectValue(boolFunction, "__call__", &q_function_bool);
    boolFunction = addObjectValue(boolFunction, "__call__argc", &oneArgc);
    addGVar("bool", boolFunction);

    Object * typeFunction = emptyObject("bfunction");
    typeFunction = addObjectValue(typeFunction, "__call__", &q_function_type);
    typeFunction = addObjectValue(typeFunction, "__call__argc", &oneArgc);
    addGVar("type", typeFunction);

    Object * dispFunction = emptyObject("bfunction");
    dispFunction = addObjectValue(dispFunction, "__call__", &q_function_display);
    dispFunction = addObjectValue(dispFunction, "__call__argc", &oneArgc);
    addGVar("disp", dispFunction);

    Object * lenFunction = emptyObject("bfunction");
    lenFunction = addObjectValue(lenFunction, "__call__", &q_function_len);
    lenFunction = addObjectValue(lenFunction, "__call__argc", &oneArgc);
    addGVar("len", lenFunction);

    Object * execFunction = emptyObject("bfunction");
    execFunction = addObjectValue(execFunction, "__call__", &q_function_exec);
    execFunction = addObjectValue(execFunction, "__call__argc", &oneArgc);
    addGVar("exec", execFunction);

    Object * sizeofFunction = emptyObject("bfunction");
    sizeofFunction = addObjectValue(sizeofFunction, "__call__", &q_function_sizeof);
    sizeofFunction = addObjectValue(sizeofFunction, "__call__argc", &oneArgc);
    addGVar("sizeof", sizeofFunction);

    Object * minFunction = emptyObject("bfunction");
    minFunction = addObjectValue(minFunction, "__call__", &q_function_min);
    minFunction = addObjectValue(minFunction, "__call__argc", &oneArgc);
    addGVar("min", minFunction);

    Object * maxFunction = emptyObject("bfunction");
    maxFunction = addObjectValue(maxFunction, "__call__", &q_function_max);
    maxFunction = addObjectValue(maxFunction, "__call__argc", &oneArgc);
    addGVar("max", maxFunction);

    Object * tocharFunction = emptyObject("bfunction");
    tocharFunction = addObjectValue(tocharFunction, "__call__", &q_function_tochar);
    tocharFunction = addObjectValue(tocharFunction, "__call__argc", &oneArgc);
    addGVar("tochar", tocharFunction);

    Object * charcodeFunction = emptyObject("bfunction");
    charcodeFunction = addObjectValue(charcodeFunction, "__call__", &q_function_charcode);
    charcodeFunction = addObjectValue(charcodeFunction, "__call__argc", &oneArgc);
    addGVar("charcode", charcodeFunction);

    /*
    The rest
    */

    Object * inputFunction = emptyObject("bfunction");
    inputFunction = addObjectValue(inputFunction, "__call__", &q_function_input);
    inputFunction = addObjectValue(inputFunction, "__call__argmin", &falsePtr);
    inputFunction = addObjectValue(inputFunction, "__call__argmax", &oneArgc);
    addGVar("input", inputFunction);
}

/*

Objects

*/

// Create a pointer to a malloc'd Object and return it
Object * objectPointer()
{
    Object * ptr = malloc(sizeof(Object));
    ptr->value_count = 0;
    ptr->refs = 0;

    return ptr;
}

Object * emptyObject(char * name)
{
    Object * self = objectPointer();

    self->name = name;

    self->names = malloc(sizeof(char *));
    self->values = malloc(sizeof(void *));
    self->value_count = 0;

    return self;
}

Object * makeObject(char * name, void * value)
{
    Object * self = objectPointer();

    self->name = name;

    self->names = malloc(sizeof(char *));
    self->values = malloc(sizeof(void *));
    self->value_count = 0;

    self = addObjectValue(self, "value", value);

    return self;
}

// Duplicate an Object
Object * objectCopy(Object * orig)
{
    Object * self = emptyObject(orig->name);

    for (int i = 0; i < orig->value_count; i++)
    {
        char * name = malloc(strlen(orig->names[i]) + 1);
        strcpy(name, orig->names[i]);
        // pushTrash(name);

        void ** newptr = malloc(sizeof(void *));
        *newptr = orig->values[i];
        // pushTrash(newptr);

        self = addObjectValue(self, name, *newptr);
    }

    // If Object * is iterable, duplicate all objects inside it
    // if (!strcmp(self->name, "list"))
    // {
    //     int length = ((int *)objectGetAttr(self, "length"))[0];
    //     Object ** lst = objectGetAttr(self, "value");
    //     for (int i = 0; i < length; i++)
    //     {
    //         // lst[i] = makeNull();//objectCopy(lst[i]);
    //     }
    // }

    return self;
}

// Summarise an Object * (for debugging - uncomment to use it)
void objectSummary(Object * obj)
{
    print("Object * of type <");
    print(obj->name);
    print(">, value_count = ");
    println(obj->value_count);
    println("-- ATTRIBUTES --");

    for (int i = 0; i < obj->value_count; i++)
    {
        print(": ");
        println(obj->names[i]);
    }
}

// Realloc an already made Object * to fit a new value into it
Object * addObjectValue(Object * obj, char * name, void * value)
{
    obj->names = realloc(obj->names, (obj->value_count + 1) * sizeof(char *));
    obj->values = realloc(obj->values, (obj->value_count + 1) * sizeof(void *));

    obj->names[obj->value_count] = name;
    obj->values[obj->value_count] = value;

    obj->value_count++;

    return obj;
}

// Add an attribute to an Object, usually during the
// initialisation of the Object * (does not use realloc)
Object * objectAddAttr(Object * obj, char * name, void * value)
{
    obj->names[obj->value_count] = name;
    obj->values[obj->value_count] = value;
    obj->value_count++;

    return obj;
}

int objectHasAttr(Object * obj, char * name)
{
    for (int i = 0; i < obj->value_count; i++)
        if (!strcmp(obj->names[i], name))
            return 1;

    return 0;
}

void * objectGetAttr(Object * obj, char * name)
{
    for (int i = 0; i < obj->value_count; i++)
        if (!strcmp(obj->names[i], name))
            return obj->values[i];

    return NULL;
}

int objectGetAttrIndex(Object * obj, char * name)
{
    for (int i = 0; i < obj->value_count; i++)
        if (!strcmp(obj->names[i], name))
            return i;

    return -1;
}


// Free an Object and all objects within (if Object is iterable)
void freeObjectR(Object * obj)
{
    // If obj is iterable, free all items within it
    if (!strcmp(obj->name, "list"))
    {
        int length = ((int *)objectGetAttr(obj, "length"))[0];
        Object ** lst = objectGetAttr(obj, "value");
        for (int i = 0; i < length; i++)
        {
            freeObjectR(lst[i]);
        }
    }

    free(obj->names);
    free(obj->values);
    free(obj);
}

// Free an Object using the __free__ attribute if the Object has one
void freeObject(Object * obj)
{
    // Call the obj.__free__ function to properly free it
    void * freeFunction = objectGetAttr(obj, "__free__");
    if (freeFunction != NULL)
    {
        Object ** arglist = makeArglist(obj);
        ((standard_func_def)freeFunction)(1, arglist);
        free(arglist);
    }

    free(obj->names);
    free(obj->values);
    free(obj);
}

// Unreference an Object
void objUnref(Object * obj)
{
    obj->refs--;

    if (obj->refs <= 0)
    {
        // println("Unreferencing");
        freeObject(obj);
    }
    // else println("Unreferencing??");
}

// Dereference an Object * (set references to 0)
void objDeref(Object * obj)
{
    // If this object is not dereferenced, dereference it
    if (obj->refs > -1)
    {
        // println("Dereferencing");
        freeObject(obj);
    }
}

/*

Memory

*/

// void pushMem(Object * obj)
// {
//     if (++memsize >= memalloc_size)
//     {
//         memalloc += memalloc_size;
//         mem = realloc(mem, memalloc * sizeof(Object *));
//     }

//     mem[memsize - 1] = obj;
// }

/*

Stack

*/

// Push to top of stack
void pushTop(Object * obj)
{
    // Increment and use new stack_size in one expression
    if (++stack_size >= stack_alloc)
    {
        stack_alloc += stack_alloc_size;
        stack = realloc(stack, stack_alloc * sizeof(Object *));
    }

    obj->refs++;
    stack[stack_size - 1] = obj;
}

// Push to top of stack and do NOT reference the Object being pushed
void pushTopM(Object * obj)
{
    // Increment and use new stack_size in one expression
    if (++stack_size >= stack_alloc)
    {
        stack_alloc += stack_alloc_size;
        stack = realloc(stack, stack_alloc * sizeof(Object *));
    }

    stack[stack_size - 1] = obj;
}

// Pop top of stack
Object * popTop()
{
    // Decrement and use new stack_size in one expression
    if (--stack_size < stack_alloc - stack_alloc_size)
    {
        stack_alloc -= stack_alloc_size;
        stack = realloc(stack, stack_alloc * sizeof(Object *));
    }

    return stack[stack_size];
}

/*

Constants

*/

// Push to top of constants
void pushConst(Object * obj)
{
    if (++constant_count >= const_alloc)
    {
        const_alloc += const_alloc_size;
        constants = realloc(constants, const_alloc * sizeof(Object *));
    }

    obj->refs++;
    constants[constant_count - 1] = obj;
}

// Pop top of constants (shouldn't really be used, but just in case)
Object * popConst()
{
    if (--constant_count < const_alloc - const_alloc_size)
    {
        const_alloc -= const_alloc_size;
        constants = realloc(constants, const_alloc * sizeof(Object *));
    }

    return constants[constant_count];
}

/*

Return Stack

*/

void pushRetTop(Object * obj)
{
    if (++ret_stack_size >= ret_stack_alloc)
    {
        ret_stack_alloc += ret_stack_alloc_size;
        ret_stack = realloc(ret_stack, ret_stack_alloc * sizeof(Object *));
    }

    ret_stack[ret_stack_size - 1] = obj;
}

Object * popRetTop()
{
    ret_stack_size--;
    return ret_stack[ret_stack_size];
}

// Var stuff
void addGVar(char * name, Object * obj)
{
    obj->refs++;

    int check = getGVarIndex(name);
    if (check != -1)
    {
        // Unreference the old variable with this name
        objUnref(globals.values[check]);

        globals.values[check] = obj;
        return;
    }

    if (++globals.count >= globals.alloc)
    {
        globals.alloc += 10;
        globals.names = realloc(globals.names, globals.alloc * sizeof(char *));
        globals.values = realloc(globals.values, globals.alloc * sizeof(Object *));
    }

    globals.names[globals.count - 1] = name;
    globals.values[globals.count - 1] = obj;
}

// addVar assigns local variables by default.
// use addGVar to assign a global variable.
void addVar(char * name, Object * obj)
{
    obj->refs++;

    int check = getLVarIndex(name);
    if (check != -1)
    {
        // Unreference the old variable with this name
        objUnref(locals.values[check]);

        locals.values[check] = obj;
        return;
    }

    if (++locals.count >= locals.alloc)
    {
        locals.alloc += 10;
        locals.names = realloc(locals.names, locals.alloc * sizeof(char *));
        locals.values = realloc(locals.values, locals.alloc * sizeof(Object *));
    }

    locals.names[locals.count - 1] = name;
    locals.values[locals.count - 1] = obj;
}

Object * getGVar(char * name)
{
    // Check globals
    for (int i = globals.offset; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    char * err = malloc(1 + strlen(name) + 37 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined as a global variable");
    error(err, line_num);

    return makeNull();
}

Object * getGVarSilent(char * name)
{
    // Check globals
    for (int i = globals.offset; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    return NULL;
}

Object * getLVar(char * name)
{
    // Check locals
    for (int i = locals.offset; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    char * err = malloc(1 + strlen(name) + 36 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined as a local variable");
    error(err, line_num);

    return makeNull();
}

Object * getLVarSilent(char * name)
{
    // Check locals
    for (int i = locals.offset; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    return NULL;
}

Object * getVar(char * name)
{
    // Check locals
    for (int i = locals.offset; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    // Check globals
    for (int i = globals.offset; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    char * err = malloc(1 + strlen(name) + 16 + 1);
    strcpy(err, "'");
    strcat(err, name);
    strcat(err, "' is not defined");
    error(err, line_num);

    return makeNull();
}

Object * getVarSilent(char * name)
{
    // Check locals
    for (int i = locals.offset; i < locals.count; i++)
        if (!strcmp(locals.names[i], name))
            return locals.values[i];

    // Check globals
    for (int i = globals.offset; i < globals.count; i++)
        if (!strcmp(globals.names[i], name))
            return globals.values[i];

    return NULL;
}

int getGVarIndex(char * name)
{
    // Check globals
    for (int i = globals.count - 1; i >= globals.offset; i--)
        if (!strcmp(globals.names[i], name))
            return i;

    return -1;
}

int getLVarIndex(char * name)
{
    // Check locals
    for (int i = locals.count - 1; i >= locals.offset; i--)
        if (!strcmp(locals.names[i], name))
            return i;

    return -1;
}

int getVarIndex(char * name)
{
    // Check locals
    for (int i = locals.count - 1; i >= locals.offset; i--)
        if (!strcmp(locals.names[i], name))
            return i;

    // Check globals
    for (int i = globals.count - 1; i >= globals.offset; i--)
        if (!strcmp(globals.names[i], name))
            return i;

    return -1;
}

void delGVarIndex(int index)
{
    objUnref(globals.values[index]);

    for (int i = index; i < locals.count; i++)
    {
        globals.names[i] = globals.names[i + 1];
        globals.values[i] = globals.values[i + 1];
    }

    globals.count--;
}

void delLVarIndex(int index)
{
    objUnref(locals.values[index]);

    for (int i = index; i < locals.count; i++)
    {
        locals.names[i] = locals.names[i + 1];
        locals.values[i] = locals.values[i + 1];
    }

    locals.count--;
}

Object ** makeArglist(Object * obj)
{
    Object ** arglist = malloc(sizeof(Object *));
    arglist[0] = obj;

    return arglist;
}

Object ** makeDoubleArglist(Object * first, Object * secnd)
{
    Object ** arglist = malloc(2 * sizeof(Object *));
    arglist[0] = first;
    arglist[1] = secnd;

    return arglist;
}

// void addMethod(Object * method)
// {
//     methods = realloc(methods, (method_count + 1) * sizeof(Object *));
//     methods[method_count] = method;
//     method_count++;
// }

//
/// Datatypes & Includes
//

#include "datatypes/function.h"
#include "datatypes/cfunction.h"
// #include "datatypes/method.h"
#include "datatypes/cmethod.h"
#include "datatypes/int.h"
#include "datatypes/long.h"
#include "datatypes/string.h"
#include "datatypes/list.h"
#include "datatypes/null.h"
#include "datatypes/module.h"

#include "datatypes/operations.h"

#include "import.h"

// Object * makeMethod(Object * (*func)(Object ** argv), int * argc)
// {
//     Object * self = objectPointer();

//     self = makeObject("method", argc);
//     self = addObjectValue(self, "__call__", &func);

//     return self;
// }

void quokka_interpret_line(char * linetext)
{
    char * waste;
    char ** line = quokka_tok(linetext, &waste);
    pushTrash(waste);

    quokka_interpret_line_tokens(line);

    free(line);
}

/*

Temporary

*/

void STACK()
{
    print("STACK : ");
    for (int i = 0; i < stack_size; i++)
    {
        Object ** arglist = makeArglist(stack[i]);

        freeObject(q_function_print(1, arglist));
        free(arglist);

        print(" : ");
        print(stack[i]->refs);

        print(", ");
    }
    println("");
}

void quokka_interpret_line_tokens(char ** line)
{
    if (!strlen(line[0]))
        return;

    // if (verbose) STACK();
    if (verbose) println(line[0]);

    if (!strcmp(line[0], "LOAD_STRING"))
    {
        char * literal_str = makeLiteralString(line[1]);

        pushConst(makeStringRaw(literal_str, 1));
    }
    else if (!strcmp(line[0], "LOAD_INT"))
    {
        long long temp = strtoll(line[1], NULL, 10);
        if (temp > INT_MAX)
        {
            long long * llptr = makeLLPtrFromStr(line[1]);

            pushConst(makeLong(llptr, 1));
        }
        else
        {
            int * intptr = makeIntPtrFromStr(line[1]);

            pushConst(makeIntRaw(intptr, 1));
        }
    }
    else if (!strcmp(line[0], "LOAD_LONG"))
    {
        long long * llptr = makeLLPtrFromStr(line[1]);

        pushConst(makeLong(llptr, 1));
    }
    if (!strcmp(line[0], "LOAD_NULL"))
    {
        pushConst(makeNullRaw());
    }
    if (!strcmp(line[0], "LOAD_CONST"))
    {
        int ind = strtol(line[1], NULL, 10);

        pushTop(constants[ind]);
    }
    if (!strcmp(line[0], "LOAD_INT_CONST"))
    {
        int ind = strtol(line[1], NULL, 10);

        pushTop(int_consts[ind]);
    }
    else if (!strcmp(line[0], "LOAD_NAME"))
    {
        pushTop(getVar(line[1]));
    }
    else if (!strcmp(line[0], "STORE_NAME"))
    {
        Object * obj = popTop();
        obj->refs--;

        addVar(line[1], obj);
    }
    else if (!strcmp(line[0], "MAKE_LIST"))
    {
        int lstsize = strtol(line[1], NULL, 10);

        Object ** value = malloc(lstsize * sizeof(Object *));

        for (int i = 0; i < lstsize; i++)
        {
            // This line previously used objectCopy(popTop())
            value[i] = popTop();
        }

        Object * lst = makeList(lstsize, value, 1);
        // lst->refs = -1;

        // Pushing to stack will reference this list
        pushTop(lst);

        free(value);
    }
    else if (!strcmp(line[0], "MAKE_LAMBDA"))
    {
        int argmin, argmax;

        if (!strcmp(line[1], "*"))
            argmin = 0;
        else
            argmin = strtol(line[1], NULL, 10);

        if (!strcmp(line[2], "*"))
            argmax = -1;
        else
            argmax = strtol(line[2], NULL, 10);

        char * f_code = expandSBrackExp(line[3]);

        char * filepath_dupe = strndup(current_file, strlen(current_file));

        pushTop(makeFunction(filepath_dupe, &f_code, argmin, argmax));
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

        char * f_code = expandSBrackExp(line[4]);

        char * filepath_dupe = strndup(current_file, strlen(current_file));

        addGVar(line[1], makeFunction(filepath_dupe, &f_code, argmin, argmax));
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
    else if (!strcmp(line[0], "MAKE_GLOBAL"))
    {
        Object * ind = getVar(line[1]);

        addGVar(line[1], ind);
    }
    else if (!strcmp(line[0], "IMPORT_UNPACK"))
    {
        while (stack_size)
        {
            Object * pathstrobj = popTop();

            if (strcmp(pathstrobj->name, "string"))
            {
                char * err = malloc(51 + strlen(pathstrobj->name) + 1 + 1);
                strcpy(err, "file path to import must be of type 'string', not '");
                strcat(err, pathstrobj->name);
                strcat(err, "'");
                error(err, line_num);
            }

            char * import_path_bare = objectGetAttr(pathstrobj, "value");

            quokka_import_standard(import_path_bare);

            objUnref(pathstrobj);
        }
    }
    else if (!strcmp(line[0], "IMPORT"))
    {
        while (stack_size)
        {
            Object * pathstrobj = popTop();

            if (strcmp(pathstrobj->name, "string"))
            {
                char * err = malloc(51 + strlen(pathstrobj->name) + 1 + 1);
                strcpy(err, "file path to import must be of type 'string', not '");
                strcat(err, pathstrobj->name);
                strcat(err, "'");
                error(err, line_num);
            }

            char * import_path_bare = objectGetAttr(pathstrobj, "value");

            char * module_name;
            Object * imported_module = quokka_import_module(&module_name, import_path_bare);

            addGVar(module_name, imported_module);

            objUnref(pathstrobj);
        }
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
    else if (!strcmp(line[0], "GET_ATTR"))
    {
        Object * obj = popTop();

        int gotten = 0;
        int invalid = 0;

        // The following type names are in alphabetical order

        if (!strcmp(obj->name, "cfunction"))
        {
            if (!strcmp(line[1], "__call__"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "cmethod"))
        {
            if (!strcmp(line[1], "parent"))
            {
                Object * parent = objectGetAttr(obj, "parent");
                pushTop(parent);

                gotten = 1;
            }
            else if (!strcmp(line[1], "__call__"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "function"))
        {
            if (!strcmp(line[1], "value"))
            {
                char * func_value = objectGetAttr(obj, "value");
                pushTop(makeString(func_value, 0));

                gotten = 1;
            }
            else if (!strcmp(line[1], "filepath"))
            {
                char * func_path = objectGetAttr(obj, "filepath");
                pushTop(makeString(func_path, 0));

                gotten = 1;
            }
            else if (!strcmp(line[1], "__call__"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "int"))
        {
            if (!strcmp(line[1], "value"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "list"))
        {
            if (!strcmp(line[1], "value"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (!strcmp(line[1], "length"))
            {
                int * lstlen = objectGetAttr(obj, "length");
                pushTop(makeInt(lstlen, 0));

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "long"))
        {
            if (!strcmp(line[1], "value"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "module"))
        {
            if (!strcmp(line[1], "value"))
            {
                char * name = objectGetAttr(obj, "value");
                pushTop(makeString(name, 0));

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }
        else if (!strcmp(obj->name, "null"))
        {
            if (!strcmp(line[1], "value"))
            {
                pushTop(obj);

                gotten = 1;
            }
            // This will always be invalid, because null is designed
            // to be a placeholder value, so there wouldn't be any
            // purpose for retrieving attributes from null
            else
                invalid = 1;
        }
        else if (!strcmp(obj->name, "string"))
        {
            if (!strcmp(line[1], "value"))
            {
                pushTop(obj);

                gotten = 1;
            }
            else if (startswith(line[1], "__"))
                invalid = 1;
        }

        if (!gotten)
        {
            void * attribute;
            if (!invalid)
                attribute = objectGetAttr(obj, line[1]);

            if (attribute == NULL || invalid)
            {
                char * err = malloc(6 + strlen(obj->name) + 20 + strlen(line[1]) + 1 + 1);
                strcpy(err, "type '");
                strcat(err, obj->name);
                strcat(err, "' has no attribute '");
                strcat(err, line[1]);
                strcat(err, "'");
                error(err, line_num);
            }

            pushTop((Object *)attribute);
        }

        // Unreference the Object that we've retrieved an attribute from
        objUnref(obj);
    }
    else if (!strcmp(line[0], "REF_TOP"))
    {
        if (stack_size)
            stack[stack_size - 1]->refs++;
    }
    else if (!strcmp(line[0], "GET_ADDRESS"))
    {
        Object * first = popTop();

        // Get pointer address as an int
        unsigned long firstaddr = intObjAddress(first);

        pushTop(makeLong(makeLLPtr(firstaddr), 1));

        objUnref(first);
    }
    else if (!strcmp(line[0], "INCREMENT"))
    {
        Object * first = getVar(line[1]);
        Object * secnd = makeInt(&truePtr, 0);

        void * func = objectGetAttr(first, "__inadd__");
        if (func != NULL)
        {
            Object ** arglist = makeDoubleArglist(first, secnd);

            ((standard_func_def)func)(2, arglist);

            // Don't unreference the constant for 1

            free(arglist);
        }
        else
        {
            func = objOperAdd(first);

            Object ** arglist = makeDoubleArglist(first, secnd);

            addVar(line[1], ((standard_func_def)func)(2, arglist));

            // Don't unreference the constant for 1

            free(arglist);
        }
    }
    else if (!strcmp(line[0], "DECREMENT"))
    {
        Object * first = popTop();
        Object * secnd = makeInt(&truePtr, 0);

        void * func = objectGetAttr(first, "__sub__");

        if (func == NULL)
        {
            char * err = malloc(6 + strlen(first->name) + 40 + 1);
            strcpy(err, "type '");
            strcat(err, first->name);
            strcat(err, "' does not have a method for subtraction");
            error(err, line_num);
        }

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));
        addVar(line[1], stack[stack_size - 1]);

        objUnref(first);
        // Don't unreference the constant for 1

        free(arglist);
    }
    else if (!strcmp(line[0], "UNARY_ADD"))
    {
        Object * first = popTop();

        void * func = objOperPos(first);

        Object ** arglist = makeArglist(first);

        pushTop(((standard_func_def)func)(1, arglist));

        objUnref(first);

        free(arglist);
    }
    else if (!strcmp(line[0], "UNARY_SUB"))
    {
        Object * first = popTop();

        void * func = objOperNeg(first);

        Object ** arglist = makeArglist(first);

        pushTop(((standard_func_def)func)(1, arglist));

        objUnref(first);

        free(arglist);
    }
    else if (!strcmp(line[0], "BINARY_ADD"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperAdd(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "BINARY_SUB"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperSub(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "BINARY_MUL"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperMul(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "BINARY_DIV"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperDiv(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "BINARY_POW"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperPow(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "INPLACE_ADD"))
    {
        Object * first = getVar(line[1]);
        Object * secnd = popTop();

        void * func = objOperInadd(first);
        if (func != NULL)
        {
            Object ** arglist = makeDoubleArglist(first, secnd);

            ((standard_func_def)func)(2, arglist);

            objUnref(secnd);

            free(arglist);
        }
        else
        {
            func = objOperAdd(first);

            Object ** arglist = makeDoubleArglist(first, secnd);

            addVar(line[1], ((standard_func_def)func)(2, arglist));

            objUnref(secnd);

            free(arglist);
        }
    }
    else if (!strcmp(line[0], "INPLACE_SUB"))
    {
        Object * first = getVar(line[1]);
        Object * secnd = popTop();

        void * func = objOperInsub(first);
        if (func != NULL)
        {
            Object ** arglist = makeDoubleArglist(first, secnd);

            ((standard_func_def)func)(2, arglist);

            objUnref(secnd);

            free(arglist);
        }
        else
        {
            func = objOperSub(first);

            Object ** arglist = makeDoubleArglist(first, secnd);

            addVar(line[1], ((standard_func_def)func)(2, arglist));

            objUnref(secnd);

            free(arglist);
        }
    }
    else if (!strcmp(line[0], "INPLACE_MUL"))
    {
        Object * first = getVar(line[1]);
        Object * secnd = popTop();

        void * func = objOperInmul(first);
        if (func != NULL)
        {
            Object ** arglist = makeDoubleArglist(first, secnd);

            ((standard_func_def)func)(2, arglist);

            objUnref(secnd);

            free(arglist);
        }
        else
        {
            func = objOperMul(first);

            Object ** arglist = makeDoubleArglist(first, secnd);

            addVar(line[1], ((standard_func_def)func)(2, arglist));

            objUnref(secnd);

            free(arglist);
        }
    }
    else if (!strcmp(line[0], "INPLACE_DIV"))
    {
        Object * first = getVar(line[1]);
        Object * secnd = popTop();

        void * func = objOperIndiv(first);
        if (func != NULL)
        {
            Object ** arglist = makeDoubleArglist(first, secnd);

            ((standard_func_def)func)(2, arglist);

            objUnref(secnd);

            free(arglist);
        }
        else
        {
            func = objOperDiv(first);

            Object ** arglist = malloc(2 * sizeof(Object *));
            arglist[0] = first;
            arglist[1] = secnd;

            addVar(line[1], ((standard_func_def)func)(2, arglist));

            objUnref(secnd);

            free(arglist);
        }
    }
    else if (!strcmp(line[0], "CMP_EQ"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperEq(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_IDENTICAL"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        // Get pointer addresses
        unsigned long firstaddr = intObjAddress(first);
        unsigned long secndaddr = intObjAddress(secnd);

        // Check if both Objects are IDENTICAL
        if (firstaddr == secndaddr)
            pushTop(makeInt(&truePtr, 0));
        else
            pushTop(makeInt(&falsePtr, 0));

        objUnref(first);
        objUnref(secnd);
    }
    else if (!strcmp(line[0], "CMP_NEQ"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperEq(first);

        // Get result from __eq__
        Object ** arglist = makeDoubleArglist(first, secnd);

        Object * condition = ((standard_func_def)func)(2, arglist);

        // Convert to bool
        arglist = realloc(arglist, sizeof(Object *));
        arglist[0] = condition;

        Object * conditionbool = q_function_bool(1, arglist);

        // Flip it, then push it to stack
        int * intptr;

        if (!((int *)objectGetAttr(conditionbool, "value"))[0])
            intptr = &truePtr;
        else
            intptr = &falsePtr;

        pushTop(makeInt(intptr, 0));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_LT"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperLt(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_GT"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperGt(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_LE"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperLe(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "CMP_GE"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        void * func = objOperGe(first);

        Object ** arglist = makeDoubleArglist(first, secnd);

        pushTop(((standard_func_def)func)(2, arglist));

        objUnref(first);
        objUnref(secnd);

        free(arglist);
    }
    else if (!strcmp(line[0], "BOOLEAN_NOT"))
    {
        Object * first = popTop();

        int firstbool;

        // Convert first to bool
        if (objectHasAttr(first, "__bool__"))
        {
            Object ** arglist = makeArglist(first);
            firstbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            firstbool = 0;

        // Perform Boolean NOT
        if (!firstbool)
            pushTop(makeInt(&truePtr, 0));
        else
            pushTop(makeInt(&falsePtr, 0));

        objUnref(first);
    }
    else if (!strcmp(line[0], "BOOLEAN_OR"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        int firstbool;
        int secndbool;

        // Convert first to bool
        if (objectHasAttr(first, "__bool__"))
        {
            Object ** arglist = makeArglist(first);
            firstbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            firstbool = 0;

        // Convert secnd to bool
        if (objectHasAttr(secnd, "__bool__"))
        {
            Object ** arglist = makeArglist(secnd);
            secndbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            secndbool = 0;

        // Perform Inclusive OR
        if (firstbool || secndbool)
            pushTop(makeInt(&truePtr, 0));
        else
            pushTop(makeInt(&falsePtr, 0));

        objUnref(first);
        objUnref(secnd);
    }
    else if (!strcmp(line[0], "BOOLEAN_XOR"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        int firstbool;
        int secndbool;

        // Convert first to bool
        if (objectHasAttr(first, "__bool__"))
        {
            Object ** arglist = makeArglist(first);
            firstbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            firstbool = 0;

        // Convert secnd to bool
        if (objectHasAttr(secnd, "__bool__"))
        {
            Object ** arglist = makeArglist(secnd);
            secndbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            secndbool = 0;

        // Perform Exclusive OR
        if (!!firstbool != !!secndbool)
            pushTop(makeInt(&truePtr, 0));
        else
            pushTop(makeInt(&falsePtr, 0));

        objUnref(first);
        objUnref(secnd);
    }
    else if (!strcmp(line[0], "BOOLEAN_AND"))
    {
        Object * first = popTop();
        Object * secnd = popTop();

        int firstbool;
        int secndbool;

        // Convert first to bool
        if (objectHasAttr(first, "__bool__"))
        {
            Object ** arglist = makeArglist(first);
            firstbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            firstbool = 0;

        // Convert secnd to bool
        if (objectHasAttr(secnd, "__bool__"))
        {
            Object ** arglist = makeArglist(secnd);
            secndbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            secndbool = 0;

        // Perform Boolean AND
        if (firstbool && secndbool)
            pushTop(makeInt(&truePtr, 0));
        else
            pushTop(makeInt(&falsePtr, 0));

        objUnref(first);
        objUnref(secnd);
    }
    else if (!strcmp(line[0], "TERNARY_IF"))
    {
        Object * first = popTop();

        int firstbool;

        // Convert first to bool
        if (objectHasAttr(first, "__bool__"))
        {
            Object ** arglist = makeArglist(first);
            firstbool = ((int *)objectGetAttr(q_function_bool(1, arglist), "value"))[0];
            free(arglist);
        }
        else
            firstbool = 0;

        // Perform Ternary IF
        if (firstbool)
        {
            char * replaced = strReplace(line[1], "\t", "\n");
            char * true_code = strSlice(replaced, 1, 1);

            _quokka_interpret(true_code);

            free(true_code);
            free(replaced);
        }
        else if (!strcmp(line[2], "[]"))
            pushTop(makeNull());
        else
        {
            char * replaced = strReplace(line[2], "\t", "\n");
            char * false_code = strSlice(replaced, 1, 1);

            _quokka_interpret(false_code);

            free(false_code);
            free(replaced);
        }

        objUnref(first);
    }
    else if (!strcmp(line[0], "JUMP_TO"))
    {
        for (int i = bc_line + 1; i < bc_line_count; i++)
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
        for (int i = bc_line - 1; i >= 0; i--)
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
        Object * obj = popTop();

        if (!objectHasAttr(obj, "__bool__"))
        {
            char * err = malloc(6 + strlen(obj->name) + 34 + 1);
            strcpy(err, "type '");
            strcat(err, obj->name);
            strcat(err, "' can not be converted into a bool");
            error(err, line_num);
        }

        Object ** arglist = makeArglist(obj);
        Object * conditionobj = q_function_bool(1, arglist);
        free(arglist);

        int condition = ((int *)objectGetAttr(conditionobj, "value"))[0];

        objUnref(conditionobj);

        // If false, don't jump
        if (!condition)
            return;

        // If true, jump
        int i;
        for (i = bc_line + 1; i < bc_line_count - 1; i++)
            if (!strcmp(bc_tokens[i], line[1]))
                break;

        bc_line = i;
    }
    else if (!strcmp(line[0], "JUMP_IF_FALSE"))
    {
        Object * obj = popTop();

        if (!objectHasAttr(obj, "__bool__"))
        {
            char * err = malloc(6 + strlen(obj->name) + 34 + 1);
            strcpy(err, "type '");
            strcat(err, obj->name);
            strcat(err, "' can not be converted into a bool");
            error(err, line_num);
        }

        Object ** arglist = makeArglist(obj);
        Object * conditionobj = q_function_bool(1, arglist);
        free(arglist);

        int condition = ((int *)objectGetAttr(conditionobj, "value"))[0];

        objUnref(conditionobj);

        // If true, don't jump
        if (condition)
            return;

        // If false, jump
        int i;
        for (i = bc_line + 1; i < bc_line_count - 1; i++)
            if (!strcmp(bc_tokens[i], line[1]))
                break;

        bc_line = i;
    }
    else if (!strcmp(line[0], "CALL") || !strcmp(line[0], "CALL_METHOD"))
    {
        int argcount = strtol(line[1], NULL, 10);

        Object ** arglist = malloc(argcount * sizeof(Object *));

        for (int i = 0; i < argcount; i++)
            arglist[argcount - i - 1] = popTop();

        Object * func = popTop(); // stack[stack_size - i - 1];

        void * call_attr = objectGetAttr(func, "__call__");

        if (call_attr == NULL)
            error("not a callable type", line_num);

        int funcmin = 0;
        int funcmax = -1;

        // Get argc (if it exists)
        void * argc_attr = objectGetAttr(func, "__call__argc");
        if (argc_attr != NULL)
        {
            funcmin = ((int *)argc_attr)[0];
            funcmax = funcmin;
        }
        else
        {
            // Get argmin (if it exists)
            void * argmin_attr = objectGetAttr(func, "__call__argmin");
            if (argmin_attr != NULL)
                funcmin = ((int *)argmin_attr)[0];

            // Get argmax (if it exists)
            void * argmax_attr = objectGetAttr(func, "__call__argmax");
            if (argmax_attr != NULL)
                funcmax = ((int *)argmax_attr)[0];
        }

        // Check if argument count given is inside the function's argument count boundaries
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

        if (!strcmp(func->name, "bfunction"))
        {
            pushTop(((standard_func_def)call_attr)(argcount, arglist));

            // Unreference all arguments passed into this function
            for (int i = 0; i < argcount; i++)
                objUnref(arglist[i]);

            // Unreference the function itself
            objUnref(func);
        }
        else
        {
            arglist = realloc(arglist, (argcount + 1) * sizeof(Object *));
            for (int i = argcount - 1; i >= 0; i--)
                arglist[i + 1] = arglist[i];
            arglist[0] = func;

            pushTop(((standard_func_def)call_attr)(argcount, arglist));

            // Unreference all arguments passed into this function (function itself included)
            for (int i = 0; i < argcount + 1; i++)
                objUnref(arglist[i]);
        }

        if (!strcmp(line[0], "CALL_METHOD"))
        {
            Object * parent = objectGetAttr(func, "parent");
            objUnref(parent);
        }

        free(arglist);
    }
    else if (!strcmp(line[0], "GET_INDEX"))
    {
        // If 'GET_INDEX *' then use __copy__

        if (!strcmp(line[1], "*"))
        {
            Object * obj = popTop();

            void * copy_attr = objOperCopy(obj);

            Object ** arglist = makeArglist(obj);

            pushTop(((standard_func_def)copy_attr)(1, arglist));

            free(arglist);

            return;
        }

        // If 'GET_INDEX 1' then get index like normal

        Object * ind = popTop();
        Object * obj = popTop();

        Object ** arglist = malloc(2 * sizeof(Object *));
        arglist[0] = obj;
        arglist[1] = ind;

        void * index_attr = objOperIndex(obj);

        pushTop(((standard_func_def)index_attr)(2, arglist));

        objUnref(ind);
        objUnref(obj);

        free(arglist);
    }
    else if (!strcmp(line[0], "SET_INDEX"))
    {
        Object * val = popTop();
        Object * ind = popTop();
        Object * obj = popTop();

        Object ** arglist = malloc(3 * sizeof(Object *));
        arglist[0] = obj;
        arglist[1] = ind;
        arglist[2] = val;

        void * setindex_attr = objOperSetindex(obj);

        val = ((standard_func_def)setindex_attr)(3, arglist);

        objUnref(val);
        objUnref(ind);
        // Don't unreference the Object that just got assigned

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

        if (isdigit(t[0])) // If line starts with a digit, interpret it as a new line number
        {
            line_num = strtol(t, NULL, 10) - 1;

            // Comment the next two lines out if there's a segfault
            // if (!can_return)
            resetStack();

            bc_line++;
            continue;
        }

        quokka_interpret_line(t);

        bc_line++;
    }

    free(tokens);
}

void _quokka_interpret(char * bytecode)
{
    /* Start */
    int old_line_num = line_num;
    int old_bc_line = bc_line;
    int old_bc_line_count = bc_line_count;
    char ** old_bc_tokens = bc_tokens;

    /* Main */
    bc_tokens = malloc(sizeof(char **));
    bc_tokens[0] = "";

    char * dupe = strdup(bytecode);

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

    quokka_interpret_tokens(bc_tokens);

    free(dupe);

    /* End */
    line_num = old_line_num;
    bc_line = old_bc_line;
    bc_line_count = old_bc_line_count;
    bc_tokens = old_bc_tokens;
}

// Interpret Quokka bytecode
void quokka_interpret(char * bytecode)
{
    /* Start */

    // Stack
    Object ** old_stack = malloc(stack_size * sizeof(Object *));
    for (int i = 0; i < stack_size; i++)
        old_stack[i] = stack[i];
    int old_stack_size = stack_size;
    // print("OLD : ");
    // println(old_stack_size);

    stack_alloc = stack_alloc_size;
    stack = realloc(stack, stack_alloc * sizeof(Object *));
    stack_size = 0;

    _quokka_interpret(bytecode);

    /* End */

    // Unreference Objects from the stack used in this interpretation
    resetStack();

    // Recreate and realign previous stack
    stack_alloc = stack_alloc_size;
    stack = realloc(stack, stack_alloc * sizeof(Object *));
    stack_size = 0;

    for (int i = 0; i < old_stack_size; i++)
        pushTopM(old_stack[i]);

    free(old_stack);
}
