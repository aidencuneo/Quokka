// Import strutil module (with packing)
Object * _strutil_import_module()
{
    char * name = malloc(7 + 1);
    strcpy(name, "strutil");

    // (name, attribute_count)
    Object * _strutil_module = makeModule(name, 9);

    // printable
    Object * _strutil_printable_string = makeString(_strutil_printable, 0);
    _strutil_printable_string->refs++;
    objectAddAttr(_strutil_module, "printable", _strutil_printable_string);

    // letters
    Object * _strutil_letters_string = makeString(_strutil_letters, 0);
    _strutil_letters_string->refs++;
    objectAddAttr(_strutil_module, "letters", _strutil_letters_string);

    // lowercase
    Object * _strutil_lowercase_string = makeString(_strutil_lowercase, 0);
    _strutil_lowercase_string->refs++;
    objectAddAttr(_strutil_module, "lowercase", _strutil_lowercase_string);

    // uppercase
    Object * _strutil_uppercase_string = makeString(_strutil_uppercase, 0);
    _strutil_uppercase_string->refs++;
    objectAddAttr(_strutil_module, "uppercase", _strutil_uppercase_string);

    // digits
    Object * _strutil_digits_string = makeString(_strutil_digits, 0);
    _strutil_digits_string->refs++;
    objectAddAttr(_strutil_module, "digits", _strutil_digits_string);

    // hexdigits
    Object * _strutil_hexdigits_string = makeString(_strutil_hexdigits, 0);
    _strutil_hexdigits_string->refs++;
    objectAddAttr(_strutil_module, "hexdigits", _strutil_hexdigits_string);

    // octdigits
    Object * _strutil_octdigits_string = makeString(_strutil_octdigits, 0);
    _strutil_octdigits_string->refs++;
    objectAddAttr(_strutil_module, "octdigits", _strutil_octdigits_string);

    // punctuation
    Object * _strutil_punctuation_string = makeString(_strutil_punctuation, 0);
    _strutil_punctuation_string->refs++;
    objectAddAttr(_strutil_module, "punctuation", _strutil_punctuation_string);

    // whitespace
    Object * _strutil_whitespace_string = makeString(_strutil_whitespace, 0);
    _strutil_whitespace_string->refs++;
    objectAddAttr(_strutil_module, "whitespace", _strutil_whitespace_string);

    return _strutil_module;
}
