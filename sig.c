#include "head.h"
#include "sig.h"

void exec_sig(char* args_list[], int args_cnt)
{
    if(args_cnt != 3)
    {
        fprintf(stderr, "Incorrect number of arguments\n");
        return;
    }

    //printf("%d", get_pid_by_jno(atoi(args_list[1])));
    if(get_pid_by_jno(atoi(args_list[1])) == -1)
    {
        fprintf(stderr, "Invalid Job ID\n");
        return;
    }
    
    kill((pid_t)get_pid_by_jno(atoi(args_list[1])), atoi(args_list[2]));
}

void handle_sig()
{
        int status;
        struct Job temp_job;
        pid_t pid = waitpid(-1, &status, 0);
        if(pid == -1)
        {
            return;
        }
        struct Job jb = get_job(pid);
        if(WIFEXITED(status))
        {
            printf("\n%s with pid = %d exited normally\n", jb.name, pid);
        }
        else
        {
            printf("\n%s with pid = %d exited abnormally\n", jb.name, pid);
        }
        printf("(Press ENTER to continue)");
        fflush(stdout);
        for(int i = jb.jno-1; i < MAX_JOBS - 1; i++)
        {
            jlist[i] = jlist[i+1];
        }
        jsize -= 1;
        jlist[MAX_JOBS-1].pid = -1;
}

void handle_c()
{
    printf("\n");
}

void handle_z()
{
    printf("\n");
}
