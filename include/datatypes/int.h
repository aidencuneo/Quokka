int SHIFT = 32;
#define BASE  ((long long)1 << SHIFT)
#define MASK  (BASE - 1)

void isummary(unsigned * arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%u ", arr[i]);
    printf("\n");
}

Object * __add___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        Object * res;

        int size_first = *(int *)argv[0]->values[1];
        int size_secnd = *(int *)argv[1]->values[1];

        if (size_first < 0)
        {
            if (size_secnd < 0)
            {
                res = qint_addition(argv[0], argv[1]);
                if (*(int *)res->values[1])
                    // If this number is real, invert it
                    *(int *)res->values[1] = -(*(int *)res->values[1]);
            }
            else
                res = qint_subtraction(argv[1], argv[0]);
        }
        else
        {
            if (size_secnd < 0)
                res = qint_subtraction(argv[0], argv[1]);
            else
                res = qint_addition(argv[0], argv[1]);
        }

        return res;

        return qint_addition(argv[0], argv[1]);
    }

    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     long long * third = makeLLPtr(first[0] + secnd[0]);

    //     return makeLong(third, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '+'");
    error(err, line_num);

    return makeNull();
}

Object * __sub___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
        return qint_subtraction(argv[0], argv[1]);

    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     long long * third = makeLLPtr(first[0] - secnd[0]);

    //     return makeLong(third, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '-'");
    error(err, line_num);

    return makeNull();
}

Object * __mul___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] * secnd[0]);

        return makeInt(third, 1, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] * secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '*'");
    error(err, line_num);

    return makeNull();
}

Object * __div___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0, 1);

        int * third = makeIntPtr(first[0] / secnd[0]);

        return makeInt(third, 1, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0, 1);

        long long * third = makeLLPtr(first[0] / secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '/'");
    error(err, line_num);

    return makeNull();
}

Object * __pow___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(ipowMath(first[0], secnd[0]));

        return makeInt(third, 1, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);

    return makeNull();
}

Object * __mod___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] % secnd[0]);

        return makeInt(third, 1, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] % secnd[0]);

        return makeInt(third, 1, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '%'");
    error(err, line_num);

    return makeNull();
}

Object * __xor___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] ^ secnd[0]);

        return makeInt(third, 1, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] ^ secnd[0]);

        return makeLong(third, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '^'");
    error(err, line_num);

    return makeNull();
}

Object * __lshift___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] << secnd[0]);

        return makeInt(third, 1, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '<<'");
    error(err, line_num);

    return makeNull();
}

Object * __rshift___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] >> secnd[0]);

        return makeInt(third, 1, 0);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '>>'");
    error(err, line_num);

    return makeNull();
}

Object * __eq___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0, 0);
        return makeInt(&falsePtr, 0, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '=='");
    error(err, line_num);

    return makeNull();
}

Object * __lt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }
    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     if (first[0] < secnd[0])
    //         return makeInt(&truePtr, 0, 1);
    //     return makeInt(&falsePtr, 0, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<'");
    error(err, line_num);

    return makeNull();
}

Object * __le___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '<='");
    error(err, line_num);

    return makeNull();
}

Object * __gt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>'");
    error(err, line_num);

    return makeNull();
}

Object * __ge___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0, 1);
        return makeInt(&falsePtr, 0, 1);
    }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' can not be compared with '>='");
    error(err, line_num);

    return makeNull();
}

Object * __sizeof___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");

    int * size = makeIntPtr(sizeof(argv[0]) + sizeof(thisvalue));

    return makeInt(size, 1, 1);
}

Object * __pos___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int * info = argv[0]->values[1];

    unsigned * newval = malloc(info[0] * sizeof(unsigned));

    for (int i = 0; i < info[0]; i++)
        newval[i] = value[i];

    return makeInt((int *)newval, info[0], info[1]);
}

Object * __neg___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];
    int absize = abs(size);

    unsigned * newval = malloc(absize * sizeof(unsigned));

    for (int i = 0; i < absize; i++)
        newval[i] = value[i];

    return makeInt(newval, absize, -intsign(size));
}

Object * __disp___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];

    if (size == 1)
        return makeString(intToStr(value[0]), 1);
    if (size == -1)
        return makeString(intToStr(-value[0]), 1);

    char * s = string_from_qint(argv[0], 10);
    return makeString(s, 1);

    // char * intstr = malloc(Q_INT_DIGIT_LEN * digits + 1);
    // strcpy(intstr, "");

    // for (int i = digits - 1; i >= 0; i--)
    // {
    //     char * buffer = malloc(Q_INT_DIGIT_LEN);
    //     snprintf(buffer, Q_INT_DIGIT_LEN + 1, "%u", value[i]);

    //     strncat(intstr, buffer, Q_INT_DIGIT_LEN + 1);
    //     free(buffer);
    // }

    // return makeString(intstr, 1);
}

Object * __bool___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];

    for (int i = 0; i < size; i++)
        if (value[i])
            return getIntConst(1);

    return getIntConst(0);
}

Object * __long___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeLong(makeLLPtr(first[0]), 1);
}

Object * __free___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int * size = argv[0]->values[1];

    free(value);
    free(size);

    return NULL;
}

Object * makeInt(int * value, int digits, int mult)
{
    // If 0 <= value < int_const_count, return the constant for this number
    if (value[0] * mult >= 0 && value[0] * mult < int_const_count && digits == 1)
    {
        int ind = value[0] * mult;
        free(value);
        return int_consts[ind];
    }

    return makeIntRaw(value, digits, mult);
}

Object * makeIntRaw(int * value, int digits, int mult)
{
    Object * self = objectPointer();

    self->name = "int";

    // 3 Attributes
    self->names = malloc(2 * sizeof(char *));
    self->values = malloc(2 * sizeof(void *));

    self = objectAddAttr(self, "value", value);
    self = objectAddAttr(self, "size", makeIntPtr(digits * mult));

    // self.size will always be (digit_count * multiplier)
    // where digit_count is the number of digits in this
    // integer, and multiplier is either 1 or -1, which
    // signifies the 'sign' (+ or -) of this integer.

    return self;
}

// Return qint sign using qint size
int intsign(int size)
{
    return size < 0 ? -1 : 1;
}

// Remove trailing 0's from a qint
void qint_normalise(Object * obj)
{
    int * value = obj->values[0];
    int size = *(int *)obj->values[1];

    int i;
    for (i = abs(size) - 1; i >= 0; i--)
        if (value[i])
            break;

    *(int *)obj->values[1] = intsign(size) * (i + 1);
}

Object * qint_addition(Object * a, Object * b)
{
    Object * z;

    int size_a = abs(*(int *)a->values[1]);
    int size_b = abs(*(int *)b->values[1]);

    int sign_a = intsign(*(int *)a->values[1]);
    int sign_b = intsign(*(int *)b->values[1]);

    unsigned * value_a = a->values[0];
    unsigned * value_b = b->values[0];

    // isummary(dig_a, size_a);
    // isummary(dig_a, size_b);

    // Make sure a is the largest
    if (size_a < size_b)
    {
        Object * temp = a;
        a = b;
        b = temp;

        int size_temp = size_a;
        size_a = size_b;
        size_b = size_temp;

        int sign_temp = sign_a;
        sign_a = sign_b;
        sign_b = sign_temp;
    }

    // printf("%d === %d\n", sign_a, sign_b);

    // // Realign expression if negatives are found
    // if (sign_a < 0 && sign_b >= 0)
    // {
    //     int size_a_old = *(int *)a->values[1];
    //     *(int *)a->values[1] = abs(size_a_old);

    //     z = qint_subtraction(b, a);

    //     *(int *)a->values[1] = size_a_old;

    //     return z;
    // }
    // else if (sign_a >= 0 && sign_b < 0)
    // {
    //     int size_b_old = *(int *)b->values[1];
    //     *(int *)b->values[1] = abs(size_b_old);

    //     z = qint_subtraction(a, b);

    //     *(int *)b->values[1] = size_b_old;

    //     return z;
    // }
    // else if (sign_a < 0 && sign_b < 0)
    // {
    //     int size_a_old = *(int *)a->values[1];
    //     *(int *)a->values[1] = abs(size_a_old);

    //     int size_b_old = *(int *)b->values[1];
    //     *(int *)b->values[1] = abs(size_b_old);

    //     z = qint_subtraction(a, b);

    //     *(int *)a->values[1] = size_a_old;
    //     *(int *)b->values[1] = size_b_old;

    //     return z;
    // }

    // printf("%d\n", *(int *)z->values[1] * sign_a * sign_b);

    z = makeIntRaw(
        malloc((size_a + 1) * sizeof(unsigned)),
        size_a + 1,
        1);

    long carry = 0;

    int i;
    for (i = 0; i < size_b; i++)
    {
        carry += value_a[i] + value_b[i];
        ((unsigned *)z->values[0])[i] = carry & MASK;
        carry >>= SHIFT;
    }

    for ( ; i < size_a; i++)
    {
        carry += value_a[i];
        ((unsigned *)z->values[0])[i] = carry & MASK;
        carry >>= SHIFT;
    }

    ((unsigned *)z->values[0])[i] = carry;

    // printf("%d - %d\n", sign_a, sign_b);

    // isummary(((unsigned *)z->values[0]), size_a + 1);

    qint_normalise(z);
    return z;
}

Object * qint_subtraction(Object * a, Object * b)
{
    Object * z;

    int size_a = abs(*(int *)a->values[1]);
    int size_b = abs(*(int *)b->values[1]);

    int sign_a = intsign(*(int *)a->values[1]);
    int sign_b = intsign(*(int *)b->values[1]);

    unsigned * value_a = a->values[0];
    unsigned * value_b = b->values[0];

    long borrow = 0;
    int sign = 1;
    int i;

    // Make sure a is the largest
    if (size_a < size_b)
    {
        sign = -1;

        Object * temp = a;
        a = b;
        b = temp;

        int size_temp = size_a;
        size_a = size_b;
        size_b = size_temp;

        int sign_temp = sign_a;
        sign_a = sign_b;
        sign_b = sign_temp;
    }
    else if (size_a == size_b)
    {
        // Find highest digit where a and b differ:
        i = size_a;

        while (--i >= 0 && value_a[i] == value_b[i]);

        if (i < 0)
            return int_consts[0];

        if (value_a[i] < value_b[i])
        {
            sign = -1;

            Object * temp = a;
            a = b;
            b = temp;
        }

        size_a = size_b = i + 1;
    }

    z = makeIntRaw(
        malloc((size_a + 1) * sizeof(unsigned)),
        size_a + 1,
        1);

    for (i = 0; i < size_a + 1; i++)
        ((unsigned *)z->values[0])[i] = 0;

    for (i = 0; i < size_b; i++)
    {
        borrow = (long)value_a[i] - value_b[i] - borrow;
        ((unsigned *)z->values[0])[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1; // Keep only one sign bit
    }

    for ( ; i < size_a; i++)
    {
        borrow = (long)value_a[i] - borrow;
        ((unsigned *)z->values[0])[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1; // Keep only one sign bit
    }

    *(int *)z->values[1] = sign_a * (size_a + 1);

    qint_normalise(z);
    return z;
}

Object * qint_muladd1(Object * obj, int n, int extra)
{
    int size = *(int *)obj->values[1]; // Old digit count

    // Increment digit count
    if (size < 0)
        (*(int *)obj->values[1])--;
    else
        (*(int *)obj->values[1])++;

    unsigned * value = obj->values[0];

    long carry = extra;

    if (obj == NULL)
        return NULL;

    int i;
    for (i = 0; i < abs(size); i++)
    {
        carry += (long)value[i] * n;
        value[i] = carry & MASK;
        carry >>= SHIFT;
    }

    value[i] = carry;

    qint_normalise(obj);
    return obj;
}

Object * qint_from_string(char * str, int base)
{
    // printf("%s (BASE %d)\n", str, base);

    int len = strlen(str);

    char * start;
    Object * obj;

    if ((base != 0 && base < 2) || base > 36)
        error("invalid base for long literal", line_num);

    if (!base)
    {
        if (str[0] != '0')
            base = 10;
        else if (str[1] == 'x' || str[1] == 'X')
            base = 16;
        else
            base = 8;
    }
    // if (base == 16 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    //     str += 2;

    int est_len = len / 2 + 1;
    obj = makeIntRaw(malloc(est_len * sizeof(unsigned)), 0, 1);

    // start = str;
    int i;
    for (i = 0; i < len; i++)
    {
        int k = -1;

        if (str[i] <= '9')
            k = str[i] - '0';
        else if (str[i] >= 'a')
            k = str[i] - 'a' + 10;
        else if (str[i] >= 'A')
            k = str[i] - 'A' + 10;
        if (k < 0 || k >= base)
            break;

        qint_muladd1(obj, base, k);
        // objUnref(obj);
        // obj = newobj;
    }

    for ( ; i < est_len; i++)
        ((unsigned *)obj->values[0])[i] = 0;

    if (!i) // str == start
    {
        objDeref(obj);
        error("no digits in integer", line_num);
        return NULL;
    }

    // int digit_count = *(int *)obj->values[1];
    // isummary(obj->values[0], digit_count);
    // printf("(BASE %lld)\n", BASE);

    // if (sign < 0 && z != NULL && z->ob_size != 0)
    //     z->ob_size = -(z->ob_size);

    return obj;

    // unsigned * digits;
    // int len = stringToDigitArray(str, &digits);

    // return makeIntRaw(digits, 1, len);
}

Object * qint_divrem1(Object * obj, int n, int * remptr)
{
    unsigned * value = obj->values[0];
    int size = *(int *)obj->values[1];

    long rem = 0;

    if (!(n > 0 && n <= MASK))
        error("nooo", line_num);

    // assert(n > 0 && n <= MASK);

    if (obj == NULL)
        return NULL;

    for (int i = abs(size); --i >= 0; )
    {
        rem = (rem << SHIFT) + value[i];
        value[i] = rem / n;
        rem %= n;
    }

    *remptr = rem;

    qint_normalise(obj);
    return obj;
}

char * string_from_qint(Object * obj, int base)
{
    register Object * a = obj;
    char * str;

    unsigned * digits = a->values[0];
    int size = *(int *)a->values[1];
    int size_a = abs(size);

    int i;
    char * p;
    int bits;

    char sign = 0;

    if (a == NULL) // if (a == NULL || !PyLong_Check(a))
        // PyErr_BadInternalCall();
        return NULL;

    if (base < 2 || base > 36)
        error("invalid base for qint", line_num);

    // assert(base >= 2 && base <= 36);

    /* Compute a rough upper bound for the length of the string */
    i = base;
    bits = 0;
    while (i > 1)
    {
        bits++;
        i >>= 1;
    }

    i = 6 + (size_a*SHIFT + bits-1) / bits;

    str = malloc(i + 1);
    // str = (PyStringObject *) PyString_FromStringAndSize((char *)0, i);

    p = str + i;
    *p = '\0';

    if (size < 0)
        sign = '-';

    // if (a->ob_size < 0)
    //     sign = '-';

    if (a->values[1] == 0)
        *--p = '0';
    else if ((base & (base - 1)) == 0)
    {
        // JRH: special case for power-of-2 bases
        int temp = digits[0];
        int bitsleft = SHIFT;
        int rem;
        int last = *(int *)a->values[1];
        int basebits = 1;

        i = base;
        while ((i >>= 1) > 1) ++basebits;

        i = 0;
        for (;;)
        {
            while (bitsleft >= basebits)
            {
                if ((temp == 0) && (i >= last - 1))
                    break;

                rem = temp & (base - 1);

                if (rem < 10)
                    rem += '0';
                else
                    rem += 'A' - 10;

                // assert(p > PyString_AS_STRING(str));

                *--p = (char) rem;
                bitsleft -= basebits;
                temp >>= basebits;
            }
            if (++i >= last)
            {
                if (temp == 0) break;
                bitsleft = 99;
                /* loop again to pick up final digits */
            }
            else
            {
                temp = (digits[i] << bitsleft) | temp;
                bitsleft += SHIFT;
            }
        }
    }
    else {
        // Py_INCREF(a);

        while (*(int *)a->values[1] != 0)
        {
            int rem;

            qint_divrem1(a, base, &rem);
            // Object * temp = qint_divrem1(a, base, &rem);

            // if (temp == NULL)
            // {
            //     // Py_DECREF(a);
            //     // Py_DECREF(str);
            //     return NULL;
            // }

            if (rem < 10)
                rem += '0';
            else
                rem += 'A' - 10;

            if (!(p > str))
                error("integer-to-string error", line_num);

            // assert(p > PyString_AS_STRING(str));

            *--p = (char) rem;
            // objUnref(a);
            // a = temp;
        }

        // Py_DECREF(a);
    }

    if (base == 8) {
        if (size_a != 0)
            *--p = '0';
    }
    else if (base == 16) {
        *--p = 'x';
        *--p = '0';
    }
    else if (base != 10) {
        *--p = '#';
        *--p = '0' + base%10;
        if (base > 10)
            *--p = '0' + base/10;
    }

    if (sign)
        *--p = sign;

    if (p != str)
    {
        char * q = str;

        if (!(p > q))
            error("integer-to-string error", line_num);

        while ((*q++ = *p++) != '\0');
        q--;
    }

    return str;
}
