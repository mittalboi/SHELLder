#include "head.h"
#include "pinfo.h"

void exec_pinfo(char* args_list[], int args_cnt, char* home_dir)
{
    pid_t pid;
    if(args_cnt == 1)
        pid = getpid();
    else if(args_cnt == 2)
        pid = (pid_t)atoi(args_list[1]);
    else if(args_cnt > 2)
    {
        fprintf(stderr, MAG"You tried to pass too many arguments.\nBut it failed!\n"RESET);
        return;
    }

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

    // Print required information from stat file
    printf("pid -- %d\n", (int)pid);

    char pStatus[1000];
    strcpy(pStatus, attrs[2]);
    if(strcmp(attrs[0], attrs[7]) == 0)
        strcat(pStatus, "+");
    printf("Process Status -- %s\n", pStatus);
    printf("memory -- %s\n", attrs[22]);

    sprintf(file_p, "/proc/%d/exe", (int)pid);
    char execPath[PATH_MAX + 1];
    memset(execPath, 0, PATH_MAX);
    int readStat = readlink(file_p, execPath, PATH_MAX);


    if(readStat == -1)
        printf("Executable Path -- Doesn't exist\n");
    else
    {
        char new_execp[PATH_MAX + 1];
        if(strncmp(home_dir, execPath, strlen(home_dir)) == 0)
        {
            new_execp[0] = '~';
            for(int i = strlen(home_dir); i < strlen(execPath); i++)
            {
                new_execp[i + 1 - strlen(home_dir)] = execPath[i];
            }
        }
        printf("Executable Path -- %s\n", new_execp);
    }
    
    fclose(proc_pnt);
    return;
}