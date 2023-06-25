#include "head.h"
#include "fbg.h"

void exec_fg(char* args_list[], int arg_cnt)
{
    if(arg_cnt != 2)
    {
        fprintf(stderr, "Incorrect number of arguments\n");
        return;
    }
    if(get_pid_by_jno(atoi(args_list[1])) == -1)
    {
        fprintf(stderr, "Invalid Job ID\n");
        return;
    }
    int pid = get_pid_by_jno(atoi(args_list[1]));
    kill((pid_t)pid, SIGCONT);
    int status;
    waitpid(pid, &status, 0);
}

void exec_bg(char* args_list[], int arg_cnt)
{
    if(arg_cnt != 2)
    {
        fprintf(stderr, "Incorrect number of arguments\n");
        return;
    }
    if(get_pid_by_jno(atoi(args_list[1])) == -1)
    {
        fprintf(stderr, "Invalid Job ID\n");
        return;
    }
    int pid = get_pid_by_jno(atoi(args_list[1]));
    kill((pid_t)pid, SIGCONT);
}