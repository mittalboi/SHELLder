#include "head.h"
#include "discover.h"

int alphasort_no_case1(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void exec_discover(char* args_list[], int args_cnt, char* home_dir)
{
    int i = 0, flag_f = 0, flag_d = 0, t = 0;

    char* file = NULL;
    char dir[1000] = ".";

    if(args_cnt > 5)
    {
        fprintf(stderr, MAG"You tried to pass too many arguments.\nBut it failed!\n"RESET);
        return;
    }
    for(int j = 1; j < args_cnt; j++)
    {
        if(strcmp(args_list[j], "-f") == 0)
            flag_f = 1;
        else if(strcmp(args_list[j], "-d") == 0)
            flag_d = 1;
        else if(strcmp(args_list[j], "-fd") == 0 || strcmp(args_list[j], "-df") == 0)
        {
            flag_f = 1;
            flag_d = 1;
        }
        else if(args_list[j][0] == '"')
        {
            file = strdup(args_list[j]+1);
            file[strlen(file) - 1] = '\0';
        }
        else
            strcpy(dir,args_list[j]);
    }
    
    //printf("%s\n", file);
    if(strcmp(dir, "~") == 0)
    {
        strcpy(dir, home_dir);
    }
    else if(dir[0] == '~')
    {
        sprintf(dir, "%s%s", home_dir, dir+1);
    }
    discover(dir, flag_f, flag_d, file);
}

void discover(char *dir, int flag_f, int flag_d, char *file)
{
    DIR *dh = opendir(dir);
    struct dirent **entry;

    long long int entries = scandir(dir, &entry, 0, alphasort_no_case1);

    for(int i = 0; i < entries; i++)
    {
        // if file input is given and matches with entry
        if (file && strcmp(entry[i]->d_name, file) == 0)
        {
            if ((flag_d - flag_f) == 0 || (flag_d && entry[i]->d_type == DT_DIR) || (flag_f && entry[i]->d_type != DT_DIR))
            {
                printf("%s/%s\n", dir, entry[i]->d_name);
            }
        }
        // if file input is given but doesn't match with entry then go through directory if it is one
        else if(file && strcmp(entry[i]->d_name, file) != 0)
        {
            if (entry[i]->d_name[0] == '.')
                continue;
            if ((flag_d - flag_f) == 0 || (flag_d && entry[i]->d_type == DT_DIR) || (flag_f))
            {
                if (entry[i]->d_type == DT_DIR)
                {
                    char *new_dir = (char *)malloc(sizeof(char) * 1024);
                    sprintf(new_dir, "%s/%s", dir, entry[i]->d_name);
                    discover(new_dir, flag_f, flag_d, file);
                    free(new_dir);
                }
            }
        }
        // if file input is not given, output everything
        else
        {
            if (entry[i]->d_name[0] == '.')
                continue;
            if ((flag_d - flag_f) == 0 || (flag_d && entry[i]->d_type == DT_DIR))
            {
                printf("%s/%s\n", dir, entry[i]->d_name);
                if (entry[i]->d_type == DT_DIR)
                {
                    char *new_dir = (char *)malloc(sizeof(char) * 1024);
                    sprintf(new_dir, "%s/%s", dir, entry[i]->d_name);
                    discover(new_dir, flag_f, flag_d, file);
                    free(new_dir);
                }
            }
            else if (flag_f)
            {
                if (entry[i]->d_type != DT_DIR)
                    printf("%s/%s\n", dir, entry[i]->d_name);
                else
                {
                    char *new_dir = (char *)malloc(sizeof(char) * 1024);
                    sprintf(new_dir, "%s/%s", dir, entry[i]->d_name);
                    discover(new_dir, flag_f, flag_d, file);
                    free(new_dir);
                }
            }
        }
    }
    return;
}