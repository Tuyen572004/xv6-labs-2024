#include "../kernel/param.h"
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

void copyArgv(char* newArgv[], char* argv[], int start){
    int index = 0;
    while(argv[start] != 0){
        newArgv[index++] = argv[start++];
    }
}

int
main(int argc, char *argv[])
{
    char* firstArg = argv[0];
    char* newArgv[MAXARG] ={0};
    char* command = 0;

    if(strcmp(firstArg,"trace") == 0){ // use trace
        int num = atoi(argv[1]);
        trace(num);
        command = argv[2];
        copyArgv(newArgv,argv,2);
    }
    else{ // no trace
        printf("%s\n",firstArg);
        command=argv[0];
        copyArgv(newArgv,argv,0);
    }
    exec(command,newArgv);
    exit(0);
}