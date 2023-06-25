#include "head.h"
#include "ls.h"

int alphasort_no_case(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void exec_ls(char* args_list[], int args_cnt, char* home_dir)
{
    char* flagsNfiles[10000];
    int flag_l = 0, flag_a = 0, t = 0;

    char* dirsNfiles[10000];

    for(int i = 1; i < args_cnt; i++)
    {
        flagsNfiles[i-1] = args_list[i];
    }

    if(args_cnt == 1)
    {
        char* dir = ".";
        ls_fd(dir, home_dir, 0);
        return;
    }

    for(int j = 0; j < args_cnt - 1; j++)
    {
        if(strcmp(flagsNfiles[j], "-l") == 0)
            flag_l = 1;

        else if(strcmp(flagsNfiles[j], "-a") == 0)
            flag_a = 1;

        else if(strcmp(flagsNfiles[j], "-la") == 0 || strcmp(flagsNfiles[j], "-al") == 0)
        {
            flag_l = 1;
            flag_a = 1;
        }
        else
        {
            dirsNfiles[t] = (char*)malloc(strlen(flagsNfiles[j]) + 1000);
            //if argument is not a flag then store as directory or file
            strcpy(dirsNfiles[t], flagsNfiles[j]);
            t++;
        }
    }
    

    dirsNfiles[t] = NULL;
    int t1 = 0;
    char *temp_dirsNfiles;
    while(t1 < t)
    {
        if(strcmp(dirsNfiles[t1], "~") == 0)
            strcpy(dirsNfiles[t1], home_dir);

        else if(dirsNfiles[t1][0] == '~')
        {
            temp_dirsNfiles = (char*)malloc(strlen(home_dir) + 1000);
            strcpy(temp_dirsNfiles, home_dir);
            strcat(temp_dirsNfiles, &dirsNfiles[t1][1]);
            strcpy(dirsNfiles[t1], temp_dirsNfiles);
            free(temp_dirsNfiles);
        }
        t1++;
    }

    if(flag_l == 1 && flag_a == 1)
    {
        if(t == 0)      //if no file or directory is given
            ls_la(getcwd(NULL, 0), home_dir, 0);
        else
        {
            for(int k = 0; k < t-1; k++)
            {
                ls_la(dirsNfiles[k], home_dir, t);
                printf("\n");
            }
            ls_la(dirsNfiles[t-1], home_dir, t);
        }
    }

    if(flag_l == 1 && flag_a == 0)
    {
        if(t == 0)      //if no file or directory is given
            ls_l(getcwd(NULL, 0), home_dir, 0);
        else
        {
            for(int k = 0; k < t-1; k++)
            {
                ls_l(dirsNfiles[k], home_dir, t);
                printf("\n");
            }
            ls_l(dirsNfiles[t-1], home_dir, t);
        }
    }

    if(flag_l == 0 && flag_a == 1)
    {
        if(t == 0)      //if no file or directory is given
            ls_a(getcwd(NULL, 0), home_dir, 0);
        else
        {
            for(int k = 0; k < t-1; k++)
            {
                ls_a(dirsNfiles[k], home_dir, t);
                printf("\n");
            }
            ls_a(dirsNfiles[t-1], home_dir, t);
        }
    }

    if(flag_l == 0 && flag_a == 0)
    {
        for(int k = 0; k < t-1; k++)
        {
            ls_fd(dirsNfiles[k], home_dir, t);
            printf("\n");
        }
        ls_fd(dirsNfiles[t-1], home_dir, t);
    }
    for(int i = 0; i < t; i++)
        free(dirsNfiles[i]);
    return;
}

void ls_la(char* dir_file, char* home_dir, int t)
{
    DIR *dir;
    struct dirent **entry;

    if(t > 1)
    {
        char *name_dir = strrchr(dir_file, '/');
        if (name_dir == NULL)
        {
            name_dir = dir_file;
        }
        printf("%s:\n", name_dir);
    }
    // (d)permission ogu - symbolic link - user - group - size - date - name

    long long int entries = scandir(dir_file, &entry, NULL, alphasort_no_case); // scan directory
    if (entries < 0)
    {
        perror("scandir"); // error
    }
    else // if no error
    {
        int total = 0;
        for (int i = 0; i < entries; i++)
        {
            char file_wd[1000];
            sprintf(file_wd, "%s/%s", dir_file, entry[i]->d_name);

            struct stat fs;
            int y = stat(file_wd, &fs);

            total += fs.st_blocks;
        }

        printf("total %d\n", total/2);
        for (int i = 0; i < entries; i++)
        {
            char file_wd[1000];
            char ogu[15];
            long long int symbolic_link;
            char user[1000];
            char group[1000];
            long long int size;
            char date[1000];
            char name[1000];

            sprintf(file_wd, "%s/%s", dir_file, entry[i]->d_name);

            struct stat fs;
            int y = stat(file_wd, &fs);

            if(y < 0)
            {
                fprintf(stderr, MAG"You tried to obtain"RED" %s."MAG"\nBut it failed!\n"RESET, file_wd);
                return;
            }

            // check if directory
            if (S_ISDIR(fs.st_mode))
                strcpy(ogu, "d");
            else
                strcpy(ogu, "-");
            // get user perms
            if (fs.st_mode & S_IRUSR)
                strcat(ogu, "r");
            else
                strcat(ogu, "-");
            
            if (fs.st_mode & S_IWUSR)
                strcat(ogu, "w");
            else
                strcat(ogu, "-");

            if (fs.st_mode & S_IXUSR)
                strcat(ogu, "x");
            else
                strcat(ogu, "-");
            // get group perms
            if (fs.st_mode & S_IRGRP)
                strcat(ogu, "r");
            else
                strcat(ogu, "-");

            if (fs.st_mode & S_IWGRP)
                strcat(ogu, "w");
            else
                strcat(ogu, "-");

            if (fs.st_mode & S_IXGRP)
                strcat(ogu, "x");
            else
                strcat(ogu, "-");
            // get other's perms
            if (fs.st_mode & S_IROTH)
                strcat(ogu, "r");
            else
                strcat(ogu, "-");

            if (fs.st_mode & S_IWOTH)
                strcat(ogu, "w");
            else
                strcat(ogu, "-");

            if (fs.st_mode & S_IXOTH)
                strcat(ogu, "x");
            else
                strcat(ogu, "-");

            // get symbolic link
            symbolic_link = fs.st_nlink;

            // get user
            struct passwd *pd;
            pd = getpwuid(fs.st_uid);

            // get group
            struct group *gr;
            gr = getgrgid(fs.st_gid);

            // get size
            size = fs.st_size;

            // get date
            strcpy(date, ctime(&fs.st_mtime)+4);
            date[12] = '\0';

            // get name
            strcpy(name, entry[i]->d_name);

            // get blocks
            //total = fs.st_blocks;

            // print
            if(S_ISDIR(fs.st_mode))
            {
                printf("%s %lld %s %s %10lld %s ", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date);
                printf(BLU"%s\n"RESET, name);
            }
            else if((S_IXUSR & fs.st_mode) || (S_IXGRP & fs.st_mode) || (S_IXOTH & fs.st_mode))
            {
                printf("%s %lld %s %s %10lld %s ", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date);
                printf(GRN"%s\n"RESET, name);
            }
            else 
                printf("%s %lld %s %s %10lld %s %s\n", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date, name);
        }
    }
}

void ls_l(char* dir_file, char* home_dir, int t)
{
    DIR *dir;
    struct dirent **entry;

    if(t > 1)
    {
        char *name_dir = strrchr(dir_file, '/');
        if (name_dir == NULL)
        {
            name_dir = dir_file;
        }
        printf("%s:\n", name_dir);
    }
    // (d)permission ogu - symbolic link - user - group - size - date - name

    long long int entries = scandir(dir_file, &entry, NULL, alphasort_no_case); // scan directory
    if (entries < 0)
    {
        perror("scandir"); // error
    }
    else // if no error
    {
        int total = 0;
        for (int i = 0; i < entries; i++)
        {
            if(entry[i]->d_name[0] != '.')
            {
                char file_wd[1000];
                sprintf(file_wd, "%s/%s", dir_file, entry[i]->d_name);

                struct stat fs;
                int y = stat(file_wd, &fs);

                total += fs.st_blocks;
            }
        }

        printf("total %d\n", total/2);
        for (int i = 0; i < entries; i++)
        {
            if (entry[i]->d_name[0] != '.')
            {
                
                char file_wd[1000];
                char ogu[15];
                long long int symbolic_link;
                char user[1000];
                char group[1000];
                long long int size;
                char date[1000];
                char name[1000];

                strcpy(file_wd, dir_file);
                strcat(file_wd, "/");
                strcat(file_wd, entry[i]->d_name);

                struct stat fs;
                int y = stat(file_wd, &fs);

                if(y < 0)
                {
                    printf("File could not be obtained\n");
                    return;
                }

                // check if directory
                if (S_ISDIR(fs.st_mode))
                    strcpy(ogu, "d");
                else
                    strcpy(ogu, "-");
                // get user perms
                if (fs.st_mode & S_IRUSR)
                    strcat(ogu, "r");
                else
                    strcat(ogu, "-");
                
                if (fs.st_mode & S_IWUSR)
                    strcat(ogu, "w");
                else
                    strcat(ogu, "-");

                if (fs.st_mode & S_IXUSR)
                    strcat(ogu, "x");
                else
                    strcat(ogu, "-");
                // get group perms
                if (fs.st_mode & S_IRGRP)
                    strcat(ogu, "r");
                else
                    strcat(ogu, "-");

                if (fs.st_mode & S_IWGRP)
                    strcat(ogu, "w");
                else
                    strcat(ogu, "-");

                if (fs.st_mode & S_IXGRP)
                    strcat(ogu, "x");
                else
                    strcat(ogu, "-");
                // get other's perms
                if (fs.st_mode & S_IROTH)
                    strcat(ogu, "r");
                else
                    strcat(ogu, "-");

                if (fs.st_mode & S_IWOTH)
                    strcat(ogu, "w");
                else
                    strcat(ogu, "-");

                if (fs.st_mode & S_IXOTH)
                    strcat(ogu, "x");
                else
                    strcat(ogu, "-");

                // get symbolic link
                symbolic_link = fs.st_nlink;

                // get user
                struct passwd *pd;
                pd = getpwuid(fs.st_uid);

                // get group
                struct group *gr;
                gr = getgrgid(fs.st_gid);

                // get size
                size = fs.st_size;

                // get date
                strcpy(date, ctime(&fs.st_mtime)+4);
                date[12] = '\0';

                // get name
                strcpy(name, entry[i]->d_name);

                // print
                if(S_ISDIR(fs.st_mode))
                {
                    printf("%s %lld %s %s %10lld %s ", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date);
                    printf(BLU"%s\n"RESET, name);
                }
                else if((S_IXUSR & fs.st_mode) || (S_IXGRP & fs.st_mode) || (S_IXOTH & fs.st_mode))
                {
                    printf("%s %lld %s %s %10lld %s ", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date);
                    printf(GRN"%s\n"RESET, name);
                }
                else
                    printf("%s %lld %s %s %10lld %s %s\n", ogu, symbolic_link, pd->pw_name, gr->gr_name, size, date, name);
            }
        }
    }
}


void ls_a(char* dir_file, char* home_dir, int t)
{
    DIR *dir;
    struct dirent **entry;

    if(t > 1)
    {
        char *name_dir = strrchr(dir_file, '/');
        if (name_dir == NULL)
        {
            name_dir = dir_file;
        }
        printf("%s:\n", name_dir);
    }

    long long int entries = scandir(dir_file, &entry, 0, alphasort_no_case);

    if (entries < 0)
    {
        perror("scandir");
    }
    else
    {
        for (int i = 0; i < entries; i++)
        {
            char file_wd[1000];
            sprintf(file_wd, "%s/%s", dir_file, entry[i]->d_name);

            struct stat fs;
            int y = stat(file_wd, &fs);
            if(S_ISDIR(fs.st_mode))
                printf(BLU"%s\n"RESET, entry[i]->d_name);
            else if((S_IXUSR & fs.st_mode) || (S_IXGRP & fs.st_mode) || (S_IXOTH & fs.st_mode))
                printf(GRN"%s\n"RESET, entry[i]->d_name);
            else
                printf("%s\n", entry[i]->d_name);
        }
    }
}

void ls_fd(char* dir_file, char* home_dir, int t)
{
    DIR *dir;
    struct dirent **entry;

    if(t > 1)
    {
        char *name_dir = strrchr(dir_file, '/');
        if (name_dir == NULL)
        {
            name_dir = dir_file;
        }
        printf("%s:\n", name_dir);
    }

    long long int entries = scandir(dir_file, &entry, 0, alphasort_no_case);

    if (entries < 0)
    {
        perror("scandir");
    }
    else
    {
        for (int i = 0; i < entries; i++)
        {
            if (entry[i]->d_name[0] != '.')
            {
                char file_wd[1000];
                sprintf(file_wd, "%s/%s", dir_file, entry[i]->d_name);

                struct stat fs;
                int y = stat(file_wd, &fs);
                if(S_ISDIR(fs.st_mode))
                    printf(BLU"%s\n"RESET, entry[i]->d_name);
                else if((S_IXUSR & fs.st_mode) || (S_IXGRP & fs.st_mode) || (S_IXOTH & fs.st_mode))
                    printf(GRN"%s\n"RESET, entry[i]->d_name);
                else
                    printf("%s\n", entry[i]->d_name);
            }
        }
    }
}