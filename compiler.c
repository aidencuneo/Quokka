#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char * text, int line)
{
    printf("\nProgram compilation terminated:\nLine %d -> %s\n\n", line + 1, text);
    exit(EXIT_FAILURE);
}

int arrsize(char * arr[])
{
    int i = 0;
    char * j = arr[0];
    while (j != '\0')
    {
        i++;
        j = arr[i];
    }
    return i;
}

int main(int argc, char ** argv)
{
    printf("--START--\n");

    if (argc < 3) return 1;

    char * buffer = 0;
    long length;
    FILE * f = fopen(argv[1], "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
            fread (buffer, 1, length, f);
        fclose(f);
    }

    if (!buffer)
        return 0;

    int i = 0;
    char * tokens[1024];
    tokens[0] = strtok(buffer, "\n");
    while (tokens[i] != NULL)
    {
        i++;
        tokens[i] = strtok(NULL, "\n");
    }

    char compiled[4096] = {0};
    for (int i = 0; i < sizeof(tokens); i++)
    {
        if (tokens[i] == NULL)
            continue;
        else if (strcmp(tokens[i], "end") == 0)
            break;

        char * line[512];
        int j = 0;
        line[0] = strtok(tokens[i], ";");
        while (line[j] != NULL)
        {
            j++;
            line[j] = strtok(NULL, ";");
        }

        printf("SIZE %d\n", arrsize(line));
        if (arrsize(line) < 1) continue;
        else if (strcmp(line[0], "push") == 0)
        {
            if (arrsize(line) < 2) error("push action missing argument", i);
            else if (arrsize(line) > 2) error("push action received too many arguments", i);
            strcat(compiled, "push(pt,");
            strcat(compiled, line[1]);
            strcat(compiled, ");");
        }
        else if (strcmp(line[0], "pop") == 0)
        {
            if (arrsize(line) > 1) error("pop action received too many arguments", i);
            strcat(compiled, "ptr=pop(pt);");
        }
        else if (strcmp(line[0], "print") == 0)
        {
            if (arrsize(line) < 2) strcat(compiled, "print((char)10);");
            else if (arrsize(line) > 2) error("print action received too many arguments", i);
            else
            {
                strcat(compiled, "print(");
                strcat(compiled, line[1]);
                strcat(compiled, ");");
            }
        }
        else error("undefined token", i);

        printf("-%s-\n", tokens[i]);
        for (int p = 0; line[p] != NULL; p++)
            printf("<%s>\n", line[p]);
    }

    const char * fileHeaders;
    const char * printFunction;
    const char * fileStackStruct;
    const char * fileStart;
    const char * fileEnd;

    fileHeaders = "#include <stdio.h>\n#include <string.h>\n#include <stdlib.h>\n";
    printFunction = "int iprint(int value){printf(\"%d\",value);return value;}\nchar cprint(char value){printf(\"%c\",value);return value;}\nchar*sprint(char*value){printf(\"%s\",value);return value;}\n#define print(value) _Generic((value),int:iprint,char:cprint,char*:sprint,default:sprint)(value)\n";
    fileStackStruct = "struct stack{int maxsize;int top;int*items;};struct stack*newStack(int capacity){struct stack*pt=(struct stack*)malloc(sizeof(struct stack));pt->maxsize=capacity;pt->top=-1;pt->items=(int*)malloc(sizeof(int)*capacity);return pt;}int size(struct stack*pt){return pt->top+1;}\nint isEmpty(struct stack*pt){return pt->top==-1;}\nint isFull(struct stack*pt){return pt->top==pt->maxsize-1;}\nvoid push(struct stack*pt,int x){if(isFull(pt))exit(EXIT_FAILURE);pt->items[++pt->top]=x;}\nint peek(struct stack*pt){if (!isEmpty(pt))return pt->items[pt->top];else return 0;}\nint pop(struct stack*pt){if(isEmpty(pt))return 0;return pt->items[pt->top--];}\n";
    fileStart = "int main(int argc,char**argv){struct stack*pt=newStack(4096);\n";
    fileEnd = "return 0;}";

    FILE * fp = fopen(argv[2], "w");
    fprintf(fp, "%s", fileHeaders);
    fprintf(fp, "%s", printFunction);
    fprintf(fp, "%s", fileStackStruct);
    fprintf(fp, "%s", fileStart);
    fprintf(fp, "%s", compiled);
    fprintf(fp, "%s", fileEnd);
    fclose(fp);

    printf("--END--\n");

    return 0;
}
