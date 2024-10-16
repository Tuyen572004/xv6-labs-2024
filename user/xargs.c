#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXCHAR 1000

char *readLine()
{
    // read each character until '\n' is found
    int size = MAXCHAR;

    char c;
    char *line = (char *)malloc(size);
    int index = 0;
    while (read(0, &c, sizeof(char)) > 0)
    {

        if (c == '\n')
        {
            break;
        }

        line[index++] = c;

        // if line is full, double the size of the line
        if (index >= size)
        {
            size *= 2;
            char *newLine = (char *)malloc(size);
            for (int j = 0; j < index; j++)
            {
                newLine[j] = line[j];
            }
            free(line);
            line = newLine;
        }
    }
    line[index] = '\0';
    return line;
}

void xargs(int argc, char *argv[])
{
    // read each line
    // append the line to the command
    // run command with each line
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }

    // init argv with the initial values are arguments of xargs from 2 to argc-1
    char *childArgv[MAXARG] = {0};
    int childArgc = 0;

    for (int i = 1; i < argc; i++)
    {
        childArgv[childArgc++] = argv[i];
    }

    // read line from standard input
    char *line = 0;

    while (1)
    {

        line = readLine();
        if (line[0] == '\0')
        {
            break;
        }
        // append the line to the command
        childArgv[childArgc++] = line;
        printf("line: %s\n", line);
        for(int i = 0; i < childArgc; i++){
            printf("childArgv[%d]: %s\n", i, childArgv[i]);
        }

        // run command with each line
        int pid = fork();
        if (pid == 0)
        {
            exec(childArgv[0], childArgv);
            free(line);
            line = 0;
            fprintf(2, "child : exec %s failed\n", childArgv[0]);
            exit(1);
            
        }
        else
        {
            wait(0);
        }
    }
}

int main(int argc, char *argv[])
{
    xargs(argc, argv);
    return 0;
}