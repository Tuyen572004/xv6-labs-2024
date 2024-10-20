#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXBUF 100

char* readLine() {
    // read each character until '\n' is found
    int size = MAXBUF;

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

void xargs(int argc, char* argv[]) {
    char *childArgv[MAXARG] = {0};
    int childArgc = 0;

    // initialize childArgv : copy all arguments except the first one
    for (int i = 1; i < argc; i++) {
        childArgv[childArgc++] = argv[i];
    }

    char *line = 0;

    while (1) {

        line = readLine();

        if (line[0] == '\0') { // have read all lines
            break;
        }
 
        childArgv[childArgc] = line; // append the line to the arguments

        int pid = fork();
        if (pid == 0) {
            exec(childArgv[0], childArgv);
            // If exec returns, there was an error
            fprintf(2, "xargs: exec %s failed\n", childArgv[0]);
            exit(2); // Exit with an error status : error in child process
        } else {
            wait(0);
        }

    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: %s <command> <args>\n", argv[0]);
        exit(1); // Exit with an error status if usage is incorrect
    }
    xargs(argc, argv);
    return 0; 
}