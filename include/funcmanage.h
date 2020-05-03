typedef struct __funcManagement_Struct__ funcManagement;

void _fm_report(funcManagement self);

struct __funcManagement_Struct__
{
    char ** names;
    int * scopes;
};

funcManagement _fm_add(funcManagement self, char * name, int value)
{
    int size = getchsize(self.names);

    self.names = realloc(self.names, size + 3 * 3 * sizeof(char *));
    self.scopes = realloc(self.scopes, size + 3 * 3 * sizeof(int));

    self.names[size] = malloc(strlen(name) + 1);
    strcpy(self.names[size], name);
    self.names[size + 1] = NULL;
    self.scopes[size] = value;

    int sssize = getchsize(self.names);

    return self;
}

int _fm_exists(funcManagement self, char * name, int value)
{
    for (int i = 0; self.names[i] != NULL; i++)
    {
        if (strcmp(self.names[i], name) == 0 && self.scopes[i] <= value)
        {
            return 1;
        }
    }

    return 0;
}

void _fm_clear(funcManagement self)
{
    for (int i = 0; self.names[i] != NULL; i++)
    {
        free(self.names[i]);
    }
    free(self.names);
    free(self.scopes);
}

void _fm_report(funcManagement self)
{
    int size = getchsize(self.names);

    if (!size)
    {
        println("{,}");
        return;
    }

    char * ret = malloc(1024);
    strcpy(ret, "{");

    for (int i = 0; self.names[i] != NULL; i++)
    {
        strcat(ret, self.names[i]);
        strcat(ret, ": ");
        char * num = malloc(11);
        sprintf(num, "%d", self.scopes[i]);
        strcat(ret, num);
        strcat(ret, ",\n");
    }

    ret[strlen(ret) - 1] = '}';
    ret[strlen(ret)] = '\0';

    println(ret);
    free(ret);
}

funcManagement _functionManager()
{
    funcManagement self;

    self.names = malloc(2 * sizeof(char *));
    self.names[0] = NULL;

    self.scopes = malloc(2 * sizeof(int));

    return self;
}
