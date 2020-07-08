int msdelay(int milliseconds)
{
    if (milliseconds < 0)
        return -1;

    clock_t start_time = ((float)clock() / CLOCKS_PER_SEC) * 1000;
    while (((float)clock() / CLOCKS_PER_SEC) * 1000 < start_time + milliseconds);

    return 0;
}

Object * _time_sleepms(int argc, Object ** argv)
{
    long delay = 0; // Millisecond delay

    // If first argument is not an integer, try to convert it into one
    if (strcmp(argv[1]->name, "int"))
    {
        Object * toint = q_function_int(1, &argv[1]);

        delay = long_from_qint(toint);
    }
    else
    {
        delay = long_from_qint(argv[1]);
    }

    if (delay < 0)
    {
        char * intstr = intToStr(delay);

        char * err = malloc(21 + strlen(intstr) + 29 + 1);
        strcpy(err, "millisecond delay of ");
        strcat(err, intstr);
        strcat(err, " is too low (< 0) for sleepms");
        error(err, line_num);
    }

    msdelay(delay);

    return makeNull();
}

Object * _time_sleep(int argc, Object ** argv)
{
    long delay = 0; // Second delay

    // If first argument is not an integer, try to convert it into one
    if (strcmp(argv[1]->name, "int"))
    {
        Object * toint = q_function_int(1, &argv[1]);

        delay = long_from_qint(toint);
    }
    else
    {
        delay = long_from_qint(argv[1]);
    }

    if (delay < 0)
    {
        char * intstr = intToStr(delay);

        char * err = malloc(21 + strlen(intstr) + 29 + 1);
        strcpy(err, "millisecond delay of ");
        strcat(err, intstr);
        strcat(err, " is too low (< 0) for sleepms");
        error(err, line_num);
    }

    msdelay(delay * 1000);

    return makeNull();
}

// Import time module (with packing)
Object * _time_import_module()
{
    char * name = malloc(4 + 1);
    strcpy(name, "time");

    // (name, attribute_count)
    Object * _time_module = makeModule(name, 2);

    // sleepms
    Object * _time_sleepms_method = makeCMethod(_time_module, &_time_sleepms, 1, 1);
    _time_sleepms_method->refs++;
    objectAddAttr(_time_module, "sleepms", _time_sleepms_method);

    // sleep
    Object * _time_sleep_method = makeCMethod(_time_module, &_time_sleep, 1, 1);
    _time_sleep_method->refs++;
    objectAddAttr(_time_module, "sleep", _time_sleep_method);

    return _time_module;
}

/* This module can not be imported without packing */
