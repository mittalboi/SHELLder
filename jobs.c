#include "head.h"
#include "jobs.h"

void save_job(char *args_list[], int args_cnt, pid_t pid)
{
    int i = 0;
    while(jlist[i].pid != -1)
        i++;

    jsize = i+1;
    jmax = i+1;

    //printf("%d\n", i);
    jlist[i].name = (char*)malloc(sizeof(char*)*10000);
    strcpy(jlist[i].name, args_list[0]);
    for(int j = 1 ; j < args_cnt; j++)
    {
        strcat(jlist[i].name, " ");
        strcat(jlist[i].name, args_list[j]);
    }

    jlist[i].pid = pid;
    jlist[i].jno = i+1;

    char file_p[20];
    sprintf(file_p, "/proc/%d/stat", (int)pid);
    char fileStat[1000] = {'\0'};

    FILE *proc_pnt;
    proc_pnt = fopen(file_p, "r");
    if (proc_pnt == NULL)
    {
        perror("File path");
        return;
    }
    fread(fileStat, 1000, 1, proc_pnt);

    char *attrs[52] = {NULL};
    attrs[0] = strtok(fileStat, " ");
    for (int i = 1; attrs[i - 1] != NULL; i++)
        attrs[i] = strtok(NULL, " ");
    if(strncmp(attrs[2], "T", 1) == 0)
    {
        jlist[i].state = 0;
    }
    else
        jlist[i].state = 1;
    
    // printf("%s\n", jlist[i].name);
    // printf("%d\n", jlist[i].pid);
}

void exec_jobs(char* args_list[], int arg_cnt)
{
    //printf("%d", jsize);
    if(arg_cnt > 3)
    {
        fprintf(stderr, "Too many arguments\n");
        return;
    }
    sort_jobs();
    int i = 0;
    while(temp_jlist[i].pid != -1)
    {
        if((arg_cnt == 1 && temp_jlist[i].state == 1) || (strcmp(args_list[1], "-r") == 0 && jlist[i].state == 1))
            printf("[%d] Running %s [%d]\n", temp_jlist[i].jno, temp_jlist[i].name, temp_jlist[i].pid);
        else if((arg_cnt == 1 && temp_jlist[i].state == 0) || (strcmp(args_list[1], "-s") == 0 && temp_jlist[i].state == 0))
            printf("[%d] Stopped %s [%d]\n", temp_jlist[i].jno, temp_jlist[i].name, temp_jlist[i].pid);
        
        i++;
    }
}

struct Job get_job(pid_t pid)
{
    int i = 0;
    while(jlist[i].pid != -1)
    {
        if(jlist[i].pid == pid)
            return jlist[i];
        i++;
    }
}

int get_pid_by_jno(int jno)
{
    int i = 0;
    while(jlist[i].pid != -1)
    {
        if(jlist[i].jno == jno)
            return jlist[i].pid;
        i++;
    }
}

void sort_jobs()
{
    int i, j;
    struct Job temp;

    for(int k = 0; k < MAX_JOBS; k++)
        temp_jlist[k] = jlist[k];
    
    for (i = 0; i < jsize - 1; i++)
    {
        for (j = 0; j < (jsize - 1-i); j++)
        {
            if (strcmp(temp_jlist[j].name, temp_jlist[j + 1].name) > 0)
            {
                temp = temp_jlist[j];
                temp_jlist[j] = temp_jlist[j + 1];
                temp_jlist[j + 1] = temp;
            } 
        }
    }
}
