#include <stdio.h>

typedef struct __scopeManagement_Struct__ scopeManagement;

int getchsize(char ** arr)
{
    int i;
    for (i = 0; arr[i] && arr[i] != NULL && strlen(arr[i]); i++);
    return i;
}

void _sc_report(scopeManagement self);

struct __scopeManagement_Struct__
{
    char ** names;
    int * scopes;
};

scopeManagement _sc_add(scopeManagement self, char * name, int value)
{
    int size = getchsize(self.names);

    self.names = realloc(self.names, size + 3 * sizeof(char *));
    self.scopes = realloc(self.scopes, size + 3 * sizeof(int));

    self.names[size] = malloc(strlen(name) + 1);
    strcpy(self.names[size], name);
    self.names[size + 1] = NULL;
    self.scopes[size] = value;

    int sssize = getchsize(self.names);

    return self;
}

scopeManagement _sc_delindex(scopeManagement self, int index)
{
    int size = getchsize(self.names);

    for (int c = index; c < size; c++)
    {
        self.names[c] = self.names[c + 1];
        self.scopes[c] = self.scopes[c + 1];
    }

    return self;
}

scopeManagement _sc_delscope(scopeManagement self, int value)
{
    int size = getchsize(self.names);

    for (int i = 0; i < size; i++)
    {
        if (self.scopes[i] >= value)
        {
            _sc_delindex(self, i);
            size--;
        }
    }

    return self;
}

int _sc_exists(scopeManagement self, char * name, int value)
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

void _sc_report(scopeManagement self)
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

scopeManagement _scopeManager()
{
    scopeManagement self;

    self.names = malloc(2 * sizeof(char *));
    self.names[0] = NULL;

    self.scopes = malloc(2 * sizeof(int));

    return self;
}
