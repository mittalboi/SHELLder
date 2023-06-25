#include "head.h"
#include "cd.h"

void exec_cd(char* args_list[], int args_cnt, char* home_dir)
{
    int i = 0;
    char* PATH_OR_DIR;

    // cd doesn not accept more than one argument
    if(args_cnt > 2)
    {
        fprintf(stderr, MAG"You tried to pass too many arguments.\nBut it failed!\n"RESET);
        return;
    }

    // Case 0 : cd
    else if(args_cnt == 1)
    {
        strcpy(pre_wd, getcwd(NULL, 0));
        i = chdir(home_dir);
        if(i == -1)
            fprintf(stderr, RED"cd"MAG":"RED"%s"MAG": No such file or directory\n"RESET, home_dir);
    }
    else if(args_cnt == 2)
    {
        PATH_OR_DIR = args_list[1];

        // Case 1 : cd ..
        if(strcmp(PATH_OR_DIR, "..") == 0)
        {
            strcpy(pre_wd, getcwd(NULL, 0));
            chdir("..");
        }

        // Case 2 : cd ~
        else if(strcmp(PATH_OR_DIR, "~") == 0)
        {
            strcpy(pre_wd, getcwd(NULL, 0));
            i = chdir(home_dir);
            if(i == -1)
                fprintf(stderr, RED"cd"MAG":"RED" %s"MAG": No such file or directory\n"RESET, PATH_OR_DIR);
        }

        // Case 3 : cd .
        else if(strcmp(PATH_OR_DIR, ".") == 0)
        {
            strcpy(pre_wd, getcwd(NULL, 0));
            chdir(".");
        }

        // Case 4 : cd -
        else if(strcmp(PATH_OR_DIR, "-") == 0)
        {
            printf("%s\n", pre_wd);
            char* temp_wd = getcwd(NULL, 0);
            i = chdir(pre_wd);
            if(i == -1)
                fprintf(stderr, RED"cd"MAG":"RED" %s"MAG": No such file or directory\n"RESET, PATH_OR_DIR);
            strcpy(pre_wd, temp_wd);
        }

        // Case 5 : cd ./<dir>
        else if(PATH_OR_DIR[0] == '.')
        {
            strcpy(pre_wd, getcwd(NULL, 0));
            i = chdir(PATH_OR_DIR);
            if(i == -1)
                fprintf(stderr, RED"cd"MAG":"RED" %s"MAG": No such file or directory\n"RESET, PATH_OR_DIR);
        }

        // Case 6 : cd ~/<dir>
        else if(PATH_OR_DIR[0] == '~')
        {
            char temp[10000];
            strcpy(temp,home_dir);
            strcat(temp,PATH_OR_DIR+1);
            strcpy(pre_wd, getcwd(NULL, 0));
            i = chdir(temp);
            if(i == -1)
                fprintf(stderr, RED"cd"MAG":"RED" %s"MAG": No such file or directory\n"RESET, PATH_OR_DIR);
        }

        // Case 7 : cd <dir>
        else 
        {
            i =  chdir(PATH_OR_DIR);
            if(i == -1)
            {
                fprintf(stderr, RED"cd"MAG":"RED" %s"MAG": No such file or directory\n"RESET, PATH_OR_DIR);
            }

        }

    }
    return;
}