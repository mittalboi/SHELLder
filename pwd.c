#include "head.h"
#include "pwd.h"

void exec_pwd()
{
    //printf("wow\n");   
    char* pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
    return;
}