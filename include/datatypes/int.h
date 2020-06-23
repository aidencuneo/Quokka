#define Q_INT_DIGIT_LEN 9
// 10 ^ Q_INT_DIGIT_LEN - 1 is the qint digit value limit

Object * __add___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] + secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] + secnd[0]);

        return makeLong(third, 1);
    }

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
    {
        int * first = objectGetAttr(argv[0], "value");
        int * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] - secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        long long * third = makeLLPtr(first[0] - secnd[0]);

        return makeLong(third, 1);
    }

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

        return makeInt(third, 1);
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
            return makeInt(&falsePtr, 0);

        int * third = makeIntPtr(first[0] / secnd[0]);

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (!secnd[0])
            return makeInt(&falsePtr, 0);

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

        return makeInt(third, 1);
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

        return makeInt(third, 1);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        int * third = makeIntPtr(first[0] % secnd[0]);

        return makeInt(third, 1);
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

        return makeInt(third, 1);
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

        return makeInt(third, 1);
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

        return makeInt(third, 1);
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
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] == secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
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
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] < secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }

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
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] <= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
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
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] > secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
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
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
    }
    else if (!strcmp(argv[1]->name, "long"))
    {
        int * first = objectGetAttr(argv[0], "value");
        long long * secnd = objectGetAttr(argv[1], "value");

        if (first[0] >= secnd[0])
            return makeInt(&truePtr, 0);
        return makeInt(&falsePtr, 0);
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

    return makeInt(size, 1);
}

Object * __pos___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeInt(makeIntPtr(first[0]), 1);
}

Object * __neg___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeInt(makeIntPtr(-first[0]), 1);
}

Object * __disp___int(int argc, Object ** argv)
{
    unsigned int * value = objectGetAttr(argv[0], "value");
    int digits = *(int *)objectGetAttr(argv[0], "digits");

    if (digits == 1)
        return makeString(intToStr(value[0]), 1);
    
    char * intstr = malloc(Q_INT_DIGIT_LEN * digits + 1);
    strcpy(intstr, "");

    for (int i = digits - 1; i >= 0; i--)
    {
        char * buffer = malloc(Q_INT_DIGIT_LEN);
        snprintf(buffer, Q_INT_DIGIT_LEN + 1, "%u", value[i]);

        strncat(intstr, buffer, Q_INT_DIGIT_LEN + 1);
        free(buffer);
    }

    return makeString(intstr, 1);
}

Object * __bool___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");

    if (thisvalue[0])
        return makeInt(&truePtr, 0);
    return makeInt(&falsePtr, 0);
}

Object * __long___int(int argc, Object ** argv)
{
    int * first = objectGetAttr(argv[0], "value");

    return makeLong(makeLLPtr(first[0]), 1);
}

Object * __free___int(int argc, Object ** argv)
{
    int * thisvalue = objectGetAttr(argv[0], "value");
    free(thisvalue);

    // return makeNull();
}

Object * makeInt(int * value, int is_malloc_ptr)
{
    // If 0 <= value < int_const_count, return the constant for this number
    if (value[0] >= 0 && value[0] < int_const_count)
    {
        int ind = value[0];
        if (is_malloc_ptr)
            free(value);
        return int_consts[ind];
    }

    return makeIntRaw(value, is_malloc_ptr, 1);
}

Object * makeIntRaw(int * value, int is_malloc_ptr, int digits)
{
    Object * self = objectPointer();

    self->name = "int";

    // 2 to 3 Attributes
    if (is_malloc_ptr)
    {
        self->names = malloc(3 * sizeof(char *));
        self->values = malloc(3 * sizeof(void *));
    }
    else
    {
        self->names = malloc(2 * sizeof(char *));
        self->values = malloc(2 * sizeof(void *));
    }

    self = objectAddAttr(self, "value", value);
    self = objectAddAttr(self, "digits", makeIntPtr(digits));

    if (is_malloc_ptr)
    {
        // __free__
        self = objectAddAttr(self, "__free__", &__free___int);
    }

    return self;
}

// void summary(char * arr, int len)
// {
//     for (int i = 0; i < len; i++)
//         printf("%d ", arr[i]);
//     printf("\n");
// }

void isummary(long * arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%ld ", arr[i]);
    printf("\n");
}

// char * arrayDivide(char * arr, int denom, int len)
// {
//     int carry = 0;
//     int base = 100; // Base of the input digits
//     int convert = base / denom;

//     for (int i = 0; i < len; i++)
//     {
//         int rem = arr[i] % denom;
//         arr[i] /= denom;
//         arr[i] += carry;

//         carry = rem * convert;
//     }

//     printf("SUM : "); summary(arr, len);
//     printf("Mod : %d\n", (int)arr[len - 1] % denom);
//     printf("Car : %d\n", carry);

//     return arr;
// }

// int stringToDigitArray(char * str, unsigned ** outptr)
// {
//     int digitlen = Q_INT_DIGIT_LEN;

//     int slen = strlen(str);
//     int len = slen / digitlen + !!(slen % digitlen);

//     unsigned * digits = malloc(len * sizeof(unsigned *));
//     int ind = 0; // Start from end of array

//     int i;
//     for (i = slen - 1; i >= digitlen; i -= digitlen)
//     {
//         digits[ind] =
//             (str[i - 8] - '0') * 1e8 +
//             (str[i - 7] - '0') * 1e7 +
//             (str[i - 6] - '0') * 1e6 +
//             (str[i - 5] - '0') * 1e5 +
//             (str[i - 4] - '0') * 1e4 +
//             (str[i - 3] - '0') * 1e3 +
//             (str[i - 2] - '0') * 1e2 +
//             (str[i - 1] - '0') * 1e1 +
//             (str[i]     - '0');

//         ind++;
//     }

//     int rem = i + 1; // Remaining digits

//     digits[ind] = 0;
//     int base = ipowMath(10, rem) / 10;

//     for (i = 0; i < rem; i++)
//     {
//         digits[ind] += (str[i] - '0') * base;
//         base /= 10;
//     }

//     *outptr = digits;

//     return len;
// }

// int containsDigits(char * arr, int len)
// {
//     for (int i = 0; i < len; i++)
//         if (arr[i] != 0)
//             return 1;

//     return 0;
// }

// unsigned int expandDigits(char * arr, int len)
// {
//     unsigned int out = 0;
//     unsigned int base = 1;
//     int trailing = 1;

//     for (int i = len - 1; i >= 0; i--)
//     {
//         if (!arr[i] && trailing)
//             continue;
//         else trailing = 0;

//         out += arr[i] * base;
//         base *= 10;
//     }

//     return out;
// }


#define SHIFT 16
#define BASE  ((long)1 << SHIFT)
#define MASK  (BASE - 1)

// Remove trailing 0's from a qint
void qint_normalise(Object * obj)
{
    int digit_c = *(int *)obj->values[1];
    long * digits = obj->values[0];

    int i;
    for (i = digit_c - 1; i >= 0; i--)
        if (digits[i])
            break;

    *(int *)obj->values[1] = i + 1;
}

Object * qint_muladd1(Object * obj, int n, int extra)
{
    int digit_c = *(int *)objectGetAttr(obj, "digits"); // Old digit count

    Object * newobj = makeIntRaw(obj->values[0], 1, digit_c + 1);

    long * digits = newobj->values[0];

    printf("BEFORE : ");
    isummary(digits, digit_c + 1);

    int carry = extra;

    if (newobj == NULL)
        return NULL;

    int i;
    for (i = 0; i < digit_c; ++i)
    {
        carry += digits[i] * n;
        digits[i] = (carry & MASK);
        carry >>= SHIFT;
    }

    digits[i] = carry;

    printf("AFTER  : ");
    isummary(digits, digit_c + 1);

    qint_normalise(newobj);
    return newobj;
}

Object * qint_from_string(char * str, int base)
{
    printf("%s (BASE %d)\n", str, base);

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

    obj = makeIntRaw(malloc(sizeof(long)), 1, 0);

    // start = str;
    int i = 0;
    for ( ; i < len; i++) {
        int k = -1;

        if (str[i] <= '9')
            k = str[i] - '0';
        else if (str[i] >= 'a')
            k = str[i] - 'a' + 10;
        else if (str[i] >= 'A')
            k = str[i] - 'A' + 10;
        if (k < 0 || k >= base)
            break;

        obj = qint_muladd1(obj, base, k);
    }

    if (!i) // str == start
    {
        objDeref(obj);
        error("no digits in long int constant", line_num);
        return NULL;
    }

    int digits = *(int *)obj->values[1];
    isummary(obj->values[0], digits);
    printf("(BASE %ld)", BASE);

    // if (sign < 0 && z != NULL && z->ob_size != 0)
    //     z->ob_size = -(z->ob_size);

    return obj;

    // unsigned * digits;
    // int len = stringToDigitArray(str, &digits);

    // return makeIntRaw(digits, 1, len);
}

}
