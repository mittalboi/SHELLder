#include "head.h"
#include "echo.h"

void exec_echo(char* args_list[], int args_cnt)
{
    for(int i = 1; i < args_cnt; i++)
    {
        printf("%s ", args_list[i]);
    }
    printf("\n");
    return;
}