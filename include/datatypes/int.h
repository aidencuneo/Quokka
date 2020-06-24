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
    int * value = argv[0]->values[0];
    int digits = *(int *)argv[0]->values[1];

    int * newval = malloc(digits * sizeof(long));

    for (int i = 0; i < digits; i++)
    {
        println(value[i]);
        newval[i] = -value[i];
    }

    isummary(newval, digits);

    return makeInt(newval, digits);
}

Object * __disp___int(int argc, Object ** argv)
{
    unsigned int * value = objectGetAttr(argv[0], "value");
    int digits = *(int *)objectGetAttr(argv[0], "digits");

    if (digits == 1)
        return makeString(intToStr(value[0]), 1);

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
    int * thisvalue = argv[0]->values[0];
    int * digits = argv[0]->values[1];

    free(thisvalue);
    free(digits);

    // return makeNull();
}

Object * makeInt(int * value, int digits)
{
    // If 0 <= value < int_const_count, return the constant for this number
    if (value[0] >= 0 && value[0] < int_const_count && digits == 1)
    {
        int ind = value[0];
        free(value);
        return int_consts[ind];
    }

    return makeIntRaw(value, digits);
}

Object * makeIntRaw(int * value, int digits)
{
    Object * self = objectPointer();

    self->name = "int";

    // 3 Attributes
    self->names = malloc(3 * sizeof(char *));
    self->values = malloc(3 * sizeof(void *));

    self = objectAddAttr(self, "value", value);
    self = objectAddAttr(self, "digits", makeIntPtr(digits));

    // __free__
    self = objectAddAttr(self, "__free__", &__free___int);

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


#define SHIFT 31
#define BASE  ((long long)1 << SHIFT)
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
    int digit_c = *(int *)obj->values[1]; // Old digit count

    *(int *)obj->values[1] = digit_c + 1;
    // Object * newobj = makeIntRaw(obj->values[0], 0, digit_c + 1);

    long * digits = obj->values[0];

    printf("BEFORE : ");
    isummary(digits, digit_c + 1);

    long carry = extra;

    if (obj == NULL)
        return NULL;

    int i;
    for (i = 0; i < digit_c; ++i)
    {
        carry += digits[i] * n;
        digits[i] = (carry & MASK);
        carry >>= SHIFT;
    }

    digits[i] = carry;

    qint_normalise(obj);

    printf("AFTER  : ");
    isummary(obj->values[0], *(int *)obj->values[1]);

    return obj;
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

    obj = makeIntRaw(calloc(len / 4 + 1, sizeof(long)), 0);

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

        qint_muladd1(obj, base, k);
        // objUnref(obj);
        // obj = newobj;
    }

    if (!i) // str == start
    {
        objDeref(obj);
        error("no digits in integer", line_num);
        return NULL;
    }

    // int digit_count = *(int *)obj->values[1];
    // isummary(obj->values[0], digit_count);
    // printf("(BASE %ld)\n", BASE);

    // if (sign < 0 && z != NULL && z->ob_size != 0)
    //     z->ob_size = -(z->ob_size);

    return obj;

    // unsigned * digits;
    // int len = stringToDigitArray(str, &digits);

    // return makeIntRaw(digits, 1, len);
}

Object * qint_divrem1(Object * obj, int n, int * remptr)
{
    // Object * newobj;

	int size = *(int *)obj->values[1];
	long rem = 0;
	
    if (!(n > 0 && n <= MASK))
        error("nooo", line_num);

	// assert(n > 0 && n <= MASK);

	if (obj == NULL)
		return NULL;

    int i;
	for (i = size; --i >= 0; ) {
		rem = (rem << SHIFT) + ((long *)obj->values[0])[i];
		((long *)obj->values[0])[i] = rem / n;
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

    long * digits = a->values[0];
    int size_a = *(int *)a->values[1];

    int i;
    char * p;
    int bits;

    // char sign = '\0';

    if (a == NULL) // if (a == NULL || !PyLong_Check(a))
        // PyErr_BadInternalCall();
        return NULL;

    if (base < 2 || base > 36)
        error("that's a nono", line_num);

    // assert(base >= 2 && base <= 36);

    /* Compute a rough upper bound for the length of the string */
    i = base;
    bits = 0;
    while (i > 1) {
        ++bits;
        i >>= 1;
    }

    i = 6 + (size_a*SHIFT + bits-1) / bits;

    str = malloc(i + 1);
    // str = (PyStringObject *) PyString_FromStringAndSize((char *)0, i);

    p = str + i;
    *p = '\0';

    // if (a->ob_size < 0)
    //     sign = '-';

    if (a->values[1] == 0)
        *--p = '0';
    else if ((base & (base - 1)) == 0)
    {
        /* JRH: special case for power-of-2 bases */
        long temp = digits[0];
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

    // if (sign)
    // 	*--p = sign;

    if (p != str)
    {
        char * q = str;
        // char * q = PyString_AS_STRING(str);

        if (!(p > q))
            error("integer-to-string error", line_num);

        while ((*q++ = *p++) != '\0');
        q--;

        // _PyString_Resize((PyObject **)&str,
        //             (int) (q - PyString_AS_STRING(str)));
    }
    return str;
}
