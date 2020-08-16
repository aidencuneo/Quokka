int SHIFT = 31;
#define BASE  ((unsigned)1 << SHIFT)
#define MASK  (BASE - 1)

// For debugging
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

        assert(res->refs > 0);
        res->refs--;
        return res;
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
        Object * res;

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a < 0)
        {
            if (size_b < 0)
                res = qint_subtraction(argv[0], argv[1]);
            else
                res = qint_addition(argv[0], argv[1]);
            if (*(int *)res->values[1])
                // If this number is real, invert it
                *(int *)res->values[1] = -(*(int *)res->values[1]);
        }
        else
        {
            if (size_b < 0)
                res = qint_addition(argv[0], argv[1]);
            else
                res = qint_subtraction(argv[0], argv[1]);
        }

        assert(res->refs > 0);
        res->refs--;
        return res;
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
        Object * z;

        unsigned * value_a = argv[0]->values[0];
        unsigned * value_b = argv[1]->values[0];
        unsigned * value_z;

        int size_a = abs(*(int *)argv[0]->values[1]);
        int size_b = abs(*(int *)argv[1]->values[1]);
        int size_z = size_a + size_b;

        int base = ((int *)argv[0]->values[1])[1];

        value_z = malloc(size_z * sizeof(unsigned));

        z = makeIntRaw(
            value_z,
            size_z,
            base);
        z->refs++;

        int i;

        for (i = 0; i < size_z; i++)
            value_z[i] = 0;

        // isummary(value_z, size_z);

        for (i = 0; i < size_a; i++)
        {
            long carry = 0;
            long f = value_a[i];

            int j;

            for (j = 0; j < size_b; j++)
            {
                carry += (long)value_z[i + j] + value_b[j] * f;
                value_z[i + j] = (carry & MASK);
                carry >>= SHIFT;
            }

            for ( ; carry != 0; j++)
            {
                // assert(i+j < z->ob_size);
                carry += (long)value_z[i + j];
                value_z[i + j] = (carry & MASK);
                carry >>= SHIFT;
            }
        }

        if (size_a < 0)
            size_z = -(size_z);
        if (size_b < 0)
            size_z = -(size_z);

        // isummary(value_z, abs(size_z));

        qint_normalise(z);

        assert(z->refs > 0);
        z->refs--;
        return z;
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
        Object * res = qint_division(argv[0], argv[1]);

        if (res == NULL)
            return getIntConst(0);

        return res;
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
    // if (!strcmp(argv[1]->name, "int"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     int * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(ipowMath(first[0], secnd[0]));

    //     return makeInt(third, 1, 1);
    // }
    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     long long * third = makeLLPtr(lpowMath(first[0], secnd[0]));

    //     return makeLong(third, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '**'");
    error(err, line_num);

    return makeNull();
}

Object * __mod___int(int argc, Object ** argv)
{
    // if (!strcmp(argv[1]->name, "int"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     int * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(first[0] % secnd[0]);

    //     return makeInt(third, 1, 1);
    // }
    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(first[0] % secnd[0]);

    //     return makeInt(third, 1, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '%'");
    error(err, line_num);

    return makeNull();
}

Object * __xor___int(int argc, Object ** argv)
{
    // if (!strcmp(argv[1]->name, "int"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     int * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(first[0] ^ secnd[0]);

    //     return makeInt(third, 1, 1);
    // }
    // else if (!strcmp(argv[1]->name, "long"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     long long * secnd = objectGetAttr(argv[1], "value");

    //     long long * third = makeLLPtr(first[0] ^ secnd[0]);

    //     return makeLong(third, 1);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 30 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '^'");
    error(err, line_num);

    return makeNull();
}

Object * __lshift___int(int argc, Object ** argv)
{
    // if (!strcmp(argv[1]->name, "int"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     int * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(first[0] << secnd[0]);

    //     return makeInt(third, 1, 0);
    // }

    char * err = malloc(17 + strlen(argv[1]->name) + 31 + 1);
    strcpy(err, "types 'int' and '");
    strcat(err, argv[1]->name);
    strcat(err, "' are invalid operands for '<<'");
    error(err, line_num);

    return makeNull();
}

Object * __rshift___int(int argc, Object ** argv)
{
    // if (!strcmp(argv[1]->name, "int"))
    // {
    //     int * first = objectGetAttr(argv[0], "value");
    //     int * secnd = objectGetAttr(argv[1], "value");

    //     int * third = makeIntPtr(first[0] >> secnd[0]);

    //     return makeInt(third, 1, 0);
    // }

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
        unsigned * value_a = objectGetAttr(argv[0], "value");
        unsigned * value_b = objectGetAttr(argv[1], "value");

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a != size_b)
            return getIntConst(0);

        assert(size_a == size_b);

        for (int i = abs(size_a) - 1; i >= 0; i--)
            if (value_a[i] != value_b[i])
                return getIntConst(0);

        return getIntConst(1);
    }

    return getIntConst(0);
}

Object * __lt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        unsigned * value_a = objectGetAttr(argv[0], "value");
        unsigned * value_b = objectGetAttr(argv[1], "value");

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a < size_b)
            return getIntConst(1);
        else if (size_a > size_b)
            return getIntConst(0);

        assert(size_a == size_b);

        for (int i = abs(size_a) - 1; i >= 0; i--)
        {
            if (value_a[i] < value_b[i])
                return getIntConst(size_a > -1);
            else if (value_a[i] > value_b[i])
                return getIntConst(size_a < 0);
        }
    }

    return getIntConst(0);
}

Object * __le___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        unsigned * value_a = objectGetAttr(argv[0], "value");
        unsigned * value_b = objectGetAttr(argv[1], "value");

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a < size_b)
            return getIntConst(1);
        else if (size_a > size_b)
            return getIntConst(0);

        assert(size_a == size_b);

        for (int i = abs(size_a) - 1; i >= 0; i--)
        {
            if (value_a[i] < value_b[i])
                return getIntConst(size_a > -1);
            else if (value_a[i] > value_b[i])
                return getIntConst(size_a < 0);
        }

        return getIntConst(1);
    }

    return getIntConst(0);
}

Object * __gt___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        unsigned * value_a = objectGetAttr(argv[0], "value");
        unsigned * value_b = objectGetAttr(argv[1], "value");

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a > size_b)
            return getIntConst(1);
        else if (size_a < size_b)
            return getIntConst(0);

        assert(size_a == size_b);

        for (int i = abs(size_a) - 1; i >= 0; i--)
        {
            if (value_a[i] > value_b[i])
                return getIntConst(size_a > -1);
            else if (value_a[i] < value_b[i])
                return getIntConst(size_a < 0);
        }
    }

    return getIntConst(0);
}

Object * __ge___int(int argc, Object ** argv)
{
    if (!strcmp(argv[1]->name, "int"))
    {
        unsigned * value_a = objectGetAttr(argv[0], "value");
        unsigned * value_b = objectGetAttr(argv[1], "value");

        int size_a = *(int *)argv[0]->values[1];
        int size_b = *(int *)argv[1]->values[1];

        if (size_a > size_b)
            return getIntConst(1);
        else if (size_a < size_b)
            return getIntConst(0);

        assert(size_a == size_b);

        for (int i = abs(size_a) - 1; i >= 0; i--)
        {
            if (value_a[i] > value_b[i])
                return getIntConst(size_a > -1);
            else if (value_a[i] < value_b[i])
                return getIntConst(size_a < 0);
        }

        return getIntConst(1);
    }

    return getIntConst(0);
}

Object * __sizeof___int(int argc, Object ** argv)
{
    int * value = argv[0]->values[0];
    int * size = argv[0]->values[1];

    return makeInt(
        makeIntPtr(sizeof(value) + sizeof(size)),
        1,
        1);
}

Object * __pos___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];
    int absize = abs(size);

    unsigned * newval = malloc(absize * sizeof(unsigned));

    for (int i = 0; i < absize; i++)
        newval[i] = value[i];

    return makeInt(newval, size, 10);
}

Object * __neg___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];
    int base = ((int *)argv[0]->values[1])[1];

    unsigned * newval = malloc(abs(size) * sizeof(unsigned));

    for (int i = 0; i < abs(size); i++)
        newval[i] = value[i];

    return makeInt(newval, -size, base);
}

Object * __disp___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int size = *(int *)argv[0]->values[1];
    int base = ((int *)argv[0]->values[1])[1];

    if (!size)
    {
        if (base == 2)
            return makeString("b0", 0);
        if (base == 8)
            return makeString("0o0", 0);
        if (base == 10)
            return makeString("0", 0);
        if (base == 16)
            return makeString("0x0", 0);
    }
    if (size == 1 && *value <= INT_MAX)
        return makeString(intToStrBase(value[0], base), 1);
    if (size == -1 && *value <= INT_MAX)
        return makeString(intToStrBase(-value[0], base), 1);

    char * s = string_from_qint(argv[0], base);
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

Object * __free___int(int argc, Object ** argv)
{
    unsigned * value = argv[0]->values[0];
    int * size = argv[0]->values[1];

    free(value);
    free(size);

    return NULL;
}

Object * makeInt(int * value, int size, int base)
{
    if (!size && base == 10)
        return getIntConst(0);

    int sign = intsign(size);

    // If 0 <= value < int_const_count, return the constant for this number
    if (size >= 0 && value[0] * sign < int_const_count && abs(size) == 1 && base == 10)
    {
        int ind = value[0];
        free(value);
        return getIntConst(ind);
    }

    return makeIntRaw(value, size, base);
}

Object * makeIntRaw(int * value, int size, int base)
{
    int * sizeptr = malloc(2 * sizeof(int));
    sizeptr[0] = size; // Can be negative for negative integers
    sizeptr[1] = base;

    Object * self = objectPointer();

    self->name = "int";

    // 3 Attributes
    self->names = malloc(2 * sizeof(char *));
    self->values = malloc(2 * sizeof(void *));

    self = objectAddAttr(self, "value", value);
    self = objectAddAttr(self, "size", sizeptr);

    // self.size will always be (digit_count * multiplier)
    // where digit_count is the number of digits in this
    // integer, and multiplier is either 1 or -1, which
    // signifies the 'sign' (+ or -) of this integer.

    return self;
}

Object * makeIntDupe(Object * old)
{
    int size = *(int *)old->values[1];
    int absize = abs(size);

    unsigned * oldvalue = old->values[0];
    unsigned * value = malloc(absize * sizeof(unsigned));

    for (int i = 0; i < absize; i++)
        value[i] = oldvalue[i];

    return makeIntRaw(value, size, ((int *)old->values[1])[1]);
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

    int base = ((int *)a->values[1])[1];

    // Make sure a is the largest
    if (size_a < size_b)
    {
        Object * temp = a;
        a = b;
        b = temp;

        int * value_temp = value_a;
        value_a = value_b;
        value_b = value_temp;

        int size_temp = size_a;
        size_a = size_b;
        size_b = size_temp;

        int sign_temp = sign_a;
        sign_a = sign_b;
        sign_b = sign_temp;
    }

    z = makeIntRaw(
        malloc((size_a + 1) * sizeof(unsigned)),
        size_a + 1,
        base);
    z->refs++;

    unsigned carry = 0;

    int i = 0;
    for ( ; i < size_b; i++)
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

    int base = ((int *)a->values[1])[1];

    unsigned borrow = 0;
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
            return getIntConst(0);

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
        base);
    z->refs++;

    for (i = 0; i < size_a + 1; i++)
        ((unsigned *)z->values[0])[i] = 0;

    for (i = 0; i < size_b; i++)
    {
        borrow = value_a[i] - value_b[i] - borrow;
        ((unsigned *)z->values[0])[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1; // Keep only one sign bit
    }

    for ( ; i < size_a; i++)
    {
        borrow = value_a[i] - borrow;
        ((unsigned *)z->values[0])[i] = borrow & MASK;
        borrow >>= SHIFT;
        borrow &= 1; // Keep only one sign bit
    }

    // printf("{}%u\n", borrow);
    // assert(!borrow);

    // ((unsigned *)z->values[0])[i + 1] = borrow;
    *(int *)z->values[1] = sign;
    ((int *)z->values[1])[1] = size_a + 1;

    qint_normalise(z);
    return z;
}

Object * qint_division(Object * a, Object * b)
{
	Object * div;
    Object * mod;

	if (qint_divmod(a, b, &div, &mod) < 0)
		return NULL;

    println(div->refs);
    println(mod->refs);
    objUnref(mod);

	return div;
}

// Divrem algorithm
Object * qint_divrem_alg(Object * v1, Object * w1, Object ** remptr)
{
    int size_v = abs(*(int *)v1->values[1]);
    int size_w = abs(*(int *)w1->values[1]);
    int size_a;

    unsigned * value_v = v1->values[0];
    unsigned * value_w = w1->values[0];
    unsigned * value_a;

    unsigned b_last = value_w[size_w - 1];
    if (b_last >= UINT_MAX - 1) // If b_last + 1 would cause an overflow, decrement it
        --b_last;
    long d = BASE / (b_last + 1);

    // println("========");
    // isummary(value_v, size_v);
    // isummary(value_w, size_w);
    // println("========");

	Object * v = qint_mul1_d(v1, d);
	Object * w = qint_mul1_d(w1, d);
    Object * a;

    // isummary(v->values[0], *(int *)v->values[1]);
    // isummary(w->values[0], *(int *)w->values[1]);
    // println("========");

    int j;
	int k;

	if (v == NULL || w == NULL)
    {
		// Py_XDECREF(v);
		// Py_XDECREF(w);
		return NULL;
	}

    v->refs = 1;

	assert(size_v >= size_w && size_w > 1); /* Assert checks by div() */
	assert(v->refs == 1); /* Since v will be used as accumulator! */
	assert(size_w == abs(*(int *)w->values[1])); /* That's how d was calculated */
	
	size_v = abs(*(int *)v->values[1]);
    size_a = abs(size_v) - abs(size_w) + 1;

    // printf("SIZE [%d]-----------------\n", size_a);

	a = makeIntRaw(
        malloc(size_a * sizeof(unsigned)),
        size_a,
        1);

    value_v = v->values[0];
    value_w = w->values[0];
    value_a = a->values[0];

    // Clear a
    // for (int i = 0; i < size_v - size_w + 1; i++)
    //     ((unsigned *)a->values[0])[i] = 0;

	for (j = size_v, k = size_a - 1; a != NULL && k >= 0; j--, k--)
    {
		unsigned vj = (j >= size_v) ? 0 : value_v[j];
		unsigned long q;
		long carry = 0;
		int i;

		// SIGCHECK({
		// 	Py_DECREF(a2);
		// 	a2 = NULL;
		// 	break;
		// })

        // printf("(1 : carry) %ld\n", carry);

		if (vj == value_w[size_w - 1])
			q = MASK;
		else
			q = ((unsigned)vj << SHIFT) + value_v[j - 1] /
				value_w[size_w - 1];

		while (value_w[size_w - 2] * q >
				((
					((unsigned)vj << SHIFT)
					+ value_v[j - 1]
					- q * value_w[size_w - 1]
				                ) << SHIFT)
				+ value_v[j - 2])
			--q;

        // printf("(2 : carry) %ld\n", carry);
		
		for (i = 0; i < size_w && i + k < size_v; i++)
        {
            unsigned long z = value_w[i] * q;
			unsigned zz = (unsigned)(z >> SHIFT);
			carry += value_v[i + k] - z
				+ ((unsigned long)zz << SHIFT);
        // printf("(2.5 : carry) %ld\n", carry);
			value_v[i + k] = carry & MASK;
        // printf("value_v[i + k] = %u\n", value_v[i + k]);
			carry = (carry >> SHIFT) - zz;
		}

        // printf("(3 : carry) %ld\n", carry);

		if (i + k < size_v)
        {
			carry += value_v[i + k];
			value_v[i + k] = 0;
		}

        // printf("(--: carry) %ld\n", carry);

		if (carry == 0)
			value_a[k] = (unsigned)q; // Probably should be (unsigned)q
		else
        {
			// assert(carry == -1);
			value_a[k] = (unsigned)q - 1;
			// carry = 0;

			for (i = 0; i < size_w && i + k < size_v; i++)
            {
				carry += value_v[i + k] + value_w[i];
				value_v[i + k] = carry & MASK;
				carry >>= SHIFT;
			}
		}
	} /* for j, k */

    // println("ENDING:");
    // isummary(value_a, *(int *)a->values[1]);
    // println("ENDING-");

	if (a == NULL)
		*remptr = NULL;
	else
    {
		qint_normalise(a);

		*remptr = qint_divrem1_d(v, d, &d);

		// d receives the (unused) remainder
		if (*remptr == NULL)
        {
            objUnref(a);
			// Py_DECREF(a);
			a = NULL;
		}
	}

	// Py_DECREF(a);
	// Py_DECREF(b);

	return a;
}

int qint_divrem(Object * a, Object * b, Object ** divptr, Object ** remptr)
{
    Object * z;

    int size_a = abs(*(int *)a->values[1]);
    int size_b = abs(*(int *)b->values[1]);
    int size_z;

    unsigned * value_a = a->values[0];
    unsigned * value_b = b->values[0];

    int base = ((int *)a->values[1])[1];

    z = makeIntRaw(
        malloc((size_a + size_b) * sizeof(unsigned)),
        0, // ?????????????
        base);

    z->refs++;

	if ((size_b == 1 || size_b == -1 || !size_b) && !*value_b)
    {
        error("attempted division by zero", line_num);
		// PyErr_SetString(PyExc_ZeroDivisionError,
		// 		"long division or modulo");
		return -1;
	}

	if (size_a < size_b ||
	   (size_a == size_b &&
	    value_a[size_a - 1] < value_b[size_b - 1]))
    {
		/* |a| < |b|. */

        objUnref(z);
		*divptr = makeIntDupe(getIntConst(0)); // Because this will be freed eventually
        a->refs++;
		*remptr = a;

		return 0;
	}

    if (size_b > 1)
    {
        size_b = 1;
        *value_b = INT_MAX;
    }

	if (size_b == 1)
    {
		long rem = 0;

        objUnref(z);

		z = qint_divrem1_d(a, value_b[0], &rem);
		if (z == NULL)
			return -1;

		*remptr = makeIntRaw(makeIntPtr(rem), 1, base);
	}
    // else
    // {
    //     objUnref(z);
    //     z = qint_divrem_alg(a, b, remptr);
    //     if (z == NULL)
    //         return -1;
    // }

    size_z = *(int *)z->values[1];

    // println("- Division algorithm returned this (size + 1):");
    // isummary(*z->values, size_z + 1);
    // println("..............................................");

	/* Set the signs.
	   The quotient z has the sign of a*b;
	   the remainder r has the sign of a,
	   so a = b*z + r. */

	if ((size_a < 0) != (size_b < 0))
		size_z = -(size_z);
	if (size_a < 0 && *(int *)((*remptr)->values[1]) != 0)
		*(int *)((*remptr)->values[1]) = -(*(int *)((*remptr)->values[1]));

	*divptr = z;

	return 0;
}

int qint_divmod(Object * a, Object * b, Object ** divptr, Object ** modptr)
{
    Object * div;
    Object * mod;

    int size_a = *(int *)a->values[1];
    int size_b = *(int *)b->values[1];

    unsigned * value_a = a->values[0];
    unsigned * value_b = b->values[0];

    long carry = 0;

    // println(abs(size_a));

    // div = makeIntRaw(
    //     malloc((abs(size_a) + 1) * sizeof(unsigned)),
    //     0,
    //     1);

    // mod = makeIntRaw(
    //     malloc((abs(size_a) + 1) * sizeof(unsigned)),
    //     0,
    //     1);

	if (qint_divrem(a, b, &div, &mod) < 0)
    {
        println("THIS SHOULDN'T HAPPEN??");
		return -1;
    }

	if (
        (*(int *)mod->values[1] < 0 && *(int *)b->values[1] > 0) ||
	    (*(int *)mod->values[1] > 0 && *(int *)b->values[1] < 0)
    )
    {

		Object * temp;
		Object * one;
		temp = qint_addition(mod, b);

		// Py_DECREF(mod);
        objUnref(mod);

		mod = temp;

		if (mod == NULL)
        {
            objUnref(div);
			return -1;
		}

		one = getIntConst(1);

		if ((temp = qint_subtraction(div, one)) == NULL)
        {

			// Py_DECREF(mod);
			// Py_DECREF(div);
			// Py_XDECREF(one);

            objUnref(mod);
            objUnref(div);

            println("THIS ALSO SHOULDN'T HAPPEN??");
			return -1;
		}

		// Py_DECREF(one);
		// Py_DECREF(div);

        objUnref(div);

		div = temp;
	}

    qint_normalise(div);
    qint_normalise(mod);

	*divptr = div;
	*modptr = mod;

    // println("---");
    // isummary(div->values[0], abs(*(int *)div->values[1]) + 1);
    // println("---");

	return 0;
}

// Doesn't modify argument 1
Object * qint_mul1_d(Object * a, long n)
{
    Object * a2 = makeIntDupe(a);

    // Object * a2 = makeIntRaw(
    //     a->values[0],
    //     abs(*(int *)a->values[1]),
    //     intsign(*(int *)a->values[1]));

	qint_muladd1(a2, n, 0);
    return a2;
}

// Modifies argument 1
void qint_muladd1(Object * obj, long n, int extra)
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
        return;

    size = abs(size);

    int i;
    for (i = 0; i < size; i++)
    {
        carry += (unsigned long)value[i] * n;
        value[i] = carry & MASK;
        carry >>= SHIFT;
    }

    value[i] = carry;

    qint_normalise(obj);
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
    if (base == 16 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        str += 2;

    int est_len = len / 2 + 1;
    obj = makeIntRaw(
        malloc(est_len * sizeof(unsigned)),
        0,
        base);

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
    }

    for ( ; i < est_len; i++)
        ((unsigned *)obj->values[0])[i] = 0;

    if (!i) // str == start
    {
        objDeref(obj);
        error("no digits in integer", line_num);
        return NULL;
    }

    if (base == 16)
        str -= 2;

    return obj;
}

// Doesn't modify argument 1
Object * qint_divrem1_d(Object * obj, int n, long * remptr)
{
    Object * z = makeIntDupe(obj);

    unsigned * value = z->values[0];
    int size = *(int *)z->values[1];

    long rem = 0;

    if (!(n > 0 && n <= MASK))
        error("qint contains digits that are out of range", line_num);

    // assert(n > 0 && n <= MASK);

    if (z == NULL)
        return NULL;

    for (int i = abs(size); --i >= 0; )
    {
        rem = (rem << SHIFT) + value[i];
        value[i] = rem / n;
        rem %= n;
    }

    *remptr = rem;

    qint_normalise(z);
    return z;
}

// Modifies argument 1
void qint_divrem1(Object * obj, int n, long * remptr)
{
    unsigned * value = obj->values[0];
    int size = *(int *)obj->values[1];

    long rem = 0;

    if (!(n > 0 && n <= MASK))
        error("qint contains digits that are out of range", line_num);

    // assert(n > 0 && n <= MASK);

    if (obj == NULL)
        return;

    for (int i = abs(size); --i >= 0; )
    {
        rem = (rem << SHIFT) + value[i];
        value[i] = rem / n;
        rem %= n;
    }

    *remptr = rem;

    qint_normalise(obj);
}

char * string_from_qint(Object * obj, int base)
{
    // The given object will be destroyed by the end of this function
    register Object * a = makeIntDupe(obj);

    char * str;

    unsigned * digits = a->values[0];
    int size = *(int *)a->values[1];
    int size_a = abs(size);

    int i;
    char * p;
    int bits;

    char sign = 0;

    if (a == NULL) // if (a == NULL || !PyLong_Check(a))
    {
        // PyErr_BadInternalCall();
        objUnref(a);
        return NULL;
    }

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
            long rem;

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
                error("int to string error", line_num);

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
            error("int to string error", line_num);

        while ((*q++ = *p++) != '\0');
        q--;
    }

    objUnref(a);

    return str;
}

// Raises a conversion error when qint can not be
// converted without overflow
long long_from_qint(Object * a)
{
    unsigned * value_a = a->values[0];
    int size_a = *(int *)a->values[1];

    int sign = intsign(size_a);
    size_a = abs(size_a);

    long result = 0;
    unsigned long base = 1;

    for (int i = 0; i < size_a; i++)
    {
        if (value_a[i] * base < 0)
            error("could not convert qint to C long", line_num);
        result += value_a[i] * base;
        base *= BASE;
    }

    return result * sign;
}
