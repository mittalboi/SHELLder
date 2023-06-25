#include "head.h"
#include "commandcmp.h"

void cmnd_cmp(char* cmnd, char* args_list[], int args_cnt, char* home_dir, int fgbg)
{
    if(fgbg != 1)           //process not in background
    {
        //duration = 0;
        if(strcmp(cmnd, "echo") == 0)
        {
            exec_echo(args_list, args_cnt);
        }

        else if(strcmp(cmnd, "pwd") == 0)
        {
            exec_pwd();
        }

        else if(strcmp(cmnd, "cd") == 0)
        {
            exec_cd(args_list, args_cnt, home_dir);
        }

        else if(strcmp(cmnd, "ls") == 0)
        {
            exec_ls(args_list, args_cnt, home_dir);
        }
        else if(strcmp(cmnd, "history") == 0)
        {
            exec_history();
        }
        else if(strcmp(cmnd, "pinfo") == 0)
        {
            exec_pinfo(args_list, args_cnt, home_dir);
        }
        else if(strcmp(cmnd, "discover") == 0)
        {
            exec_discover(args_list, args_cnt, home_dir);
        }
        else if(strcmp(cmnd, "jobs") == 0)
        {
            exec_jobs(args_list, args_cnt);
        }
        else if(strcmp(cmnd, "sig") == 0)
        {
            exec_sig(args_list, args_cnt);
        }
        else if(strcmp(cmnd, "fg") == 0)
        {
            exec_fg(args_list, args_cnt);
        }
        else
        {
            time_t begin, end;

            int id = fork();
            if(id < 0)
            {
                perror("Fork:");
                return;
            }
            begin = time(NULL);
            
            if(id != 0)
            {

                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(0, id);
                curr_pid = id;
                int status;
                waitpid(id, &status, 0);
                if(WIFSTOPPED(status))
                {
                    save_job(args_list, args_cnt, id);
                    printf("[%d] %d\n", jsize, id);

                }
                tcsetpgrp(0, getpgid(0));
                signal(SIGTTOU, SIG_DFL);
            }
            else
            {
                setpgid(0, 0);
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                int  j = 0;
                execvp(cmnd, args_list);
                fprintf(stderr, "Error has occured with %s\n", cmnd);
                exit(1);
            }
            end = time(NULL);
            duration = end - begin;
        }
    }
    else       //process runs in background
    {
            if(strcmp(cmnd, "echo") == 0)
            {
                exec_echo(args_list, args_cnt);
            }

            else if(strcmp(cmnd, "pwd") == 0)
            {
                exec_pwd();
            }

            else if(strcmp(cmnd, "cd") == 0)
            {
                exec_cd(args_list, args_cnt, home_dir);
            }

            else if(strcmp(cmnd, "ls") == 0)
            {
                exec_ls(args_list, args_cnt, home_dir);
            }
            else if(strcmp(cmnd, "history") == 0)
            {
                exec_history();
            }
            else if(strcmp(cmnd, "pinfo") == 0)
            {
                exec_pinfo(args_list, args_cnt, home_dir);
            }
            else if(strcmp(cmnd, "discover") == 0)
            {
                exec_discover(args_list, args_cnt, home_dir);
            }
            else if(strcmp(cmnd, "jobs") == 0)
            {
                exec_jobs(args_list, args_cnt);
            }
            else if(strcmp(cmnd, "sig") == 0)
            {
                exec_sig(args_list, args_cnt);
            }
            else if(strcmp(cmnd, "fg") == 0)
            {
                exec_fg(args_list, args_cnt);
            }
            else
            {
                int id = fork();
                if(id < 0)
                {
                    perror("Fork:");
                    return;
                }
                if(id != 0)
                {

                    curr_pid = id;
                    save_job(args_list, args_cnt, id);
                    printf("[%d] %d\n", jsize, id);
                }
                if(id == 0)
                {
                    setpgid(0, 0);
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execvp(cmnd, args_list);
                    fprintf(stderr, "Error has occured with %s\n", cmnd);
                    exit(1);
                }
                
            }
    }
}

