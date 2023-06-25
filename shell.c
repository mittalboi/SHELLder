#include "head.h"
#include "jobs.h"

#define MAX_CMND 10246

char* pre_wd;
time_t duration;
FILE* fp;
char* histarry[20];
int hix;
int p[2];
struct Job jlist[MAX_JOBS];
struct Job temp_jlist[MAX_JOBS];
int jsize, jmax;
char* curr_proc;
pid_t par_pid;
pid_t curr_pid;

int main()
{
    pre_wd = (char*)malloc(sizeof(char)*10000);
    printf("A wild SHELLder appeared!\n");
    char *username = getlogin();
    if(!username)
    {
        printf("Username could not be obtained\n");
    }

    char hostname[256];

    int host_err = gethostname(hostname, sizeof(hostname)/sizeof(char));
    if(host_err)
    {
        printf("System name could not be obtained\n");
    }

    char *home_dir = getcwd(NULL, 0);
    strcpy(pre_wd, home_dir);
    int size_home = strlen(home_dir);
    //printf("%d", size_home);
    if(!home_dir)
    {
        printf("Current work directory could not be obtained");
    }

    // stdin and stdout
    int fd_in = dup(STDIN_FILENO);
    int fd_out = dup(STDOUT_FILENO);

    for(int i = 0; i < 20; i++)
    {
        histarry[i] = (char*)malloc(PATH_MAX);
        strcpy(histarry[i],"");
    }

    char* hist_path = (char*)malloc(sizeof(char)*1000);

    sprintf(hist_path, "%s/history.txt", home_dir);
    load_history(hist_path);

    for(int i = 0; i < MAX_JOBS; i++)
    {
        jlist[i].pid = -1;
    }

    par_pid = getpid();

    char *inp = malloc(sizeof(char) * 100);
    char c;


    while(1)
    {
        //duration = 0;
        int exit_num = 0;
        char *cwd = getcwd(NULL, 0);
        char ncwd[strlen(cwd)];

        //char pre_wd[10000];        // for "cd -"
        //pre_wd[len(pre_wd)] = '\0';

        int i;
    
        if(strncmp(cwd, home_dir, size_home) == 0)
        {
            ncwd[0] = '~';
            for(i = size_home; i < strlen(cwd); i++)
            {
                ncwd[i + 1 - size_home] = cwd[i];
                //printf("%c\n", ncwd[i + 1 - size_home]);
            }
            ncwd[i+1-size_home] = '\0';
        }
        else
        {
            strcpy(ncwd, cwd);
        }

        save_history(hist_path);

        // resetting stdin and stdout
        dup2(fd_in, STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        int new_fd = STDIN_FILENO;
        p[0] = p[1] = -1;

        signal(SIGCHLD, handle_sig);
        signal(SIGTSTP, handle_z);
        signal(SIGINT, handle_c);

        setbuf(stdout, NULL);
        enableRawMode();

        //signal()
        print_cli(username, hostname, ncwd);
        duration = 0;
        ////////////////////////////////////////////////////////

        char inp[PATH_MAX+1];
        char commands[PATH_MAX];

        //fgets(inp, PATH_MAX, stdin);

        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) 
        {
            if (iscntrl(c)) 
            {
                if (c == 10) 
                {
                    inp[pt] = '\0';
                    break;
                }
                else if (c == 127) 
                {
                    if (pt > 0) 
                    {
                        if (inp[pt-1] == 9) 
                        {
                            for (int i = 0; i < 7; i++) 
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9) 
                {
                    //printf("here\n");
                    int diff = autocomplete(inp, username, hostname, ncwd, home_dir);
                    //printf("here\n");
                    pt += diff;

                } 
                else if (c == 4) {
                    exit(0);
                }
                else 
                {
                    printf("%d\n", c);
                }
            } 
            else 
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        printf("\n");

        int k;
        for(k = 0; k < 20; k++)
        {
            if(strcmp(histarry[k], "") == 0)
                break;
        }
        if(k == 0) 
        {
            edit_history(inp);
        }
        else if(strcmp(histarry[k-1], inp) != 0)
        {
            edit_history(inp);
        }

        if(strcmp(inp, "\0") == 0)
        {
            continue;
        }
        i = 0;
        int j = 0;

        while (inp[i] != '\0')
        {
            //removing all whitespaces of length > 1
            if (!(inp[i] == ' ' && inp[i + 1] == ' '))
            {
                commands[j] = inp[i];               // commands holds the complete command inputted in shell
                j++;
            }
            i++;
        }

        commands[j] = '\0';
        ///////////////////////////////////////////////////////////////////////
        char *ptr1, *ptr2, *ptr3, *token1, *token2, *token3, *temp_tok;
        char *cmnd_list[MAX_CMND];      //array of char pointers
        int count = 0;
        int fgbg[MAX_CMND];     //stores whether process at index i is fg or bg

        strcat(commands, " "); //appending a space at the end
        //printf("%s3\n", commands);

        token1 = strtok_r(commands, ";", &ptr1);       //cmnd_list[i] = "echo hello world"
        token2 = strtok_r(token1, "&", &ptr2);
        temp_tok = token2;
        while((token2 = strtok_r(NULL, "&", &ptr2)) != NULL)
        {
            cmnd_list[count] = strtok_r(temp_tok, "|", &ptr3);
            while((token3 = strtok_r(NULL, "|", &ptr3)) != NULL)
            {
                fgbg[count] = 2;
                count++;
                cmnd_list[count] = token3;
            }
            fgbg[count] = 1;
            count++;
            temp_tok = token2;
        }
        cmnd_list[count] = strtok_r(temp_tok, "|", &ptr3);
        while((token3 = strtok_r(NULL, "|", &ptr3)) != NULL)
        {
            fgbg[count] = 2;
            count++;
            cmnd_list[count] = token3;
        }
        fgbg[count] = 0;
        count++;
        
        while ((token1 = strtok_r(NULL, ";", &ptr1)) != NULL)
        {
            token2 = strtok_r(token1, "&", &ptr2);
            temp_tok = token2;
            while((token2 = strtok_r(NULL, "&", &ptr2)) != NULL)
            {
                cmnd_list[count] = strtok_r(temp_tok, "|", &ptr3);
                while((token3 = strtok_r(NULL, "|", &ptr3)) != NULL)
                {
                    fgbg[count] = 2;
                    count++;
                    cmnd_list[count] = token3;
                }
                fgbg[count] = 1;
                count++;
                temp_tok = token2;
            }
            cmnd_list[count] = strtok_r(temp_tok, "|", &ptr3);
            while((token3 = strtok_r(NULL, "|", &ptr3)) != NULL)
            {
                fgbg[count] = 2;
                count++;
                cmnd_list[count] = token3;
            }
            fgbg[count] = 0;
            count++;
        }
        

        ////////////////////////////////////////////////////////////////////
        for (int i = 0; i < count; i++)
        {
            // curr_proc = (char*)malloc(sizeof(char)*10000);
            // strcpy(curr_proc, cmnd_list[i]);
            char cmnd[10000];
            int space_check = 0;
            for (int j = 0; j < strlen(cmnd_list[i]); j++)
            {
                cmnd[j] = cmnd_list[i][j];
                if(cmnd_list[i][j] == ' ')
                    space_check = 1;
            }
            cmnd[strlen(cmnd_list[i])] = '\0';
            
            char *temp = cmnd;
            char cmnd_first[10000];
            // if cli input is just one command like 'pwd' or 'cd'
            // because strtok in the else block does not function properly if no delim is found 
            if(space_check == 0)
            {
                strcpy(cmnd_first, cmnd);
            }
            else
            {
                //printf("%s6\n", inp);
                char* x = strtok(temp, " ");
                if(x != NULL)
                    strcpy(cmnd_first, x); // Get first argument
                //printf("%s6\n", x);
                else
                    continue;
            }
            
            //redirection of input/output
            int flag_in = 0;
            int flag_out = 0;
            int arg_cnt= 0;

            char* args_list[10000];
            char* temp_list = strdup(cmnd_list[i]);
            char* token = strtok(temp_list, " ");
            while(token != NULL)
            {
                args_list[arg_cnt] = token;
                arg_cnt++;
                token = strtok(NULL, " ");
            }
            args_list[arg_cnt] = NULL;


            //printf("%d\n", k);
            /* for (int j = 0; j < arg_cnt; j++)
            {
                printf("%s\n", args_list[j]);
            } */
            for (int j = 0; j < arg_cnt; j++)
            {
                //INPUT REDIRECTION
                if(args_list[j][0] == '<' && flag_in == 0)
                {
                    char *temp = args_list[j];
                    char *x = strtok(temp, "<");
                    char *y = strtok(NULL, "<");
                    if(y == NULL && j < arg_cnt - 1)
                    {
                        //printf("hello\n");
                        y = (char*)malloc(sizeof(char)*1000);
                        strcpy(y, args_list[j+1]);

                        for(int k = j; k < arg_cnt - 2; k++)
                        {
                            args_list[k] = args_list[k+2];
                        }
                        j--;
                        arg_cnt -= 2;
                    }
                    else if(y == NULL && j == arg_cnt - 1)
                    {
                        fprintf(stderr, "Input file not provided\n");
                        continue;
                    }
                    else if(y != NULL)
                    {
                        for(int k = j; k < arg_cnt - 1; k++)
                        {
                            args_list[k] = args_list[k+1];
                        }
                        j--;
                        arg_cnt--;
                    }

                    int fd = open(y, O_RDONLY);
                    if(fd < 0)
                    {
                        perror("Input file");
                        exit(1);
                    }
                    dup2(fd, 0);
                    close(fd);
                    flag_in = 1;
                }
                else if(args_list[j][0] == '>' && flag_out == 0)
                {
                    //OUTPUT APPEND
                    if(args_list[j][1] == '>')
                    {
                        //printf("henlo\n");
                        char *temp = args_list[j];
                        char *y = strtok(temp, ">>");
                        //char *x = strtok(NULL, ">>");
                        if(y == NULL && j < arg_cnt - 1)
                        {
                            y = (char*)malloc(sizeof(char)*1000);
                            strcpy(y, args_list[j+1]);

                            for(int k = j; k < arg_cnt - 2; k++)
                            {
                                args_list[k] = args_list[k+2];
                            }
                            j--;
                            arg_cnt -= 2;
                        }
                        else if(y == NULL && j == arg_cnt - 1)
                        {
                            fprintf(stderr, "Output file not provided\n");
                            exit(1);
                        }
                        else if(y != NULL)
                        {
                            for(int k = j; k < arg_cnt - 1; k++)
                            {
                                args_list[k] = args_list[k+1];
                            }
                            j--;
                            arg_cnt--;
                        }
                        int fd = open(y, O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if(fd < 0)
                        {
                            perror("Output file");
                            exit(1);
                        }
                        dup2(fd, 1);
                        close(fd);
                        free(y);
                    }
                    //OUTPUT REDIRECTION
                    else
                    {
                        // printf("are you here?\n");
                        char* temp = args_list[j];
                        char* x = strtok(temp, ">");
                        char* y = NULL;
                        // char *y = strtok(NULL, ">");
                        // printf("%s\n", args_list[j]);
                        if(x == NULL && j < arg_cnt - 1)
                        {
                            y = strdup(args_list[j+1]);

                            for(int k = j; k < arg_cnt - 2; k++)
                            {
                                args_list[k] = args_list[k+2];
                            }
                            j--;
                            arg_cnt -= 2;
                        }
                        else if(x == NULL && j == arg_cnt - 1)
                        {
                            fprintf(stderr, "Output file not provided\n");
                            exit(1);
                        }
                        else if(x != NULL)
                        {
                            y = strdup(x);
                            for(int k = j; k < arg_cnt - 1; k++)
                            {
                                args_list[k] = args_list[k+1];
                            }
                            j--;
                            arg_cnt--;
                        }
                        int fd = open(y, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if(fd < 0)
                        {
                            perror("Output file");
                            exit(1);
                        }
                        dup2(fd, 1); 
                        close(fd);
                        free(y);
                    }
                    flag_out = 1;
                }
                args_list[arg_cnt] = NULL;
            }
            // for (int j = 0; j < arg_cnt; j++)
            // {
            //     fprintf(stderr, "%s\n", args_list[j]);
            // }
            char cmnd_to_exec[10000];
            for (int k = 0; k < strlen(cmnd_first); k++)
            {
                cmnd_to_exec[k] = cmnd_first[k];
            }
            cmnd_to_exec[strlen(cmnd_first)] = '\0';

            if (strcmp(cmnd_to_exec, "exit") == 0)
            {
                save_history(hist_path);
                exit_num = -1;
                break;
            }

            else
            {
                dup2(new_fd, STDIN_FILENO);
                new_fd = STDIN_FILENO;
                if(fgbg[i] == 2)
                {
                    if(pipe(p) < 0)
                    {
                        perror("pipe");
                        exit(1);
                    }
                    new_fd = p[0];
                    dup2(p[1], STDOUT_FILENO);
                }

                cmnd_cmp(cmnd_to_exec, args_list, arg_cnt, home_dir, fgbg[i]);

                close(p[1]);
                dup2(fd_out, STDOUT_FILENO);
            }

        } 

        if(exit_num == -1)
        {
            printf(RED "EXITING SHELL" RESET "\n\n");
            break;
        }
    }

    free(hist_path);
    free(pre_wd);
    free(inp);
    for(int i = 0; i < 20; i++)
    {
        free(histarry[i]);
    }
    return 0;
}