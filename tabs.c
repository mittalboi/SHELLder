#include "head.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int autocomplete(char *inp, char* username, char* hostname, char* ncwd, char* home_dir) 
{
    char *dir = ".";
    char *all_files[500];
    int count = 0;
    DIR *d = opendir(dir);

    if(strcmp(inp, "\0") == 0)
    {
        printf("\n");
        ls_fd(".",home_dir,0);
        print_cli(username, hostname, ncwd);
        return 0;
    }
    int j = 0;
    char *cmnd_list[100];
    for(int i = 0; i < 100; i++)
        cmnd_list[i] = (char*)malloc(sizeof(char)*100);
    char *tempt = (char*)malloc(sizeof(char)*100);
    strcpy(tempt, inp);
    char* token = strtok(tempt, " ");
    strcpy(cmnd_list[0], token);
    char* buf = (char*)malloc(sizeof(char)*100);
    while((token = strtok(NULL, " "))!=NULL)
    {
        j++;
        strcpy(cmnd_list[j], token);
    }
    strcpy(buf, cmnd_list[j]);
    int pos_i = strlen(buf);
    int pos = pos_i;

    //printf("%d\n", j);


    if (d) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(buf, dir->d_name, pos) == 0) {
                all_files[count] = (char*)malloc(sizeof(char) * 100);
                strcpy(all_files[count], dir->d_name);
                count++;
            }
        }
        if(count == 0)
        {
            printf("\n");
            print_cli(username, hostname, ncwd);
            printf("%s", inp);
            return 0;
        }
        else if(count == 1) {
            printf("\n");
            strcpy(buf, all_files[0]);
            strcat(buf, " ");
            pos = strlen(buf);
        }
        else if(count > 1) 
        {
            char chr;
            int flag = 1;
            int temp = pos;
            int min_len = strlen(all_files[0]);
            printf("\n");
            for(int i = 0; i < count; i++)
            {
                if(strlen(all_files[i]) < min_len)
                    min_len = strlen(all_files[i]);
                printf("%s\n", all_files[i]);
            }
            while(1)
            {
                chr = all_files[0][temp];
                for(int i = 1; i < count; i++)
                {
                    if(chr != all_files[i][temp])
                    {
                        flag = 0;
                        break;
                    }
                }
                if(flag == 0)
                    break;
                buf[temp] = chr;
                temp++;
                if(temp > min_len)
                    break;
            }
            pos = temp;
            buf[pos] = '\0';
        }
        print_cli(username, hostname, ncwd);
        if(j > 0)
        {
            //printf("here.txt");
            strcpy(inp, cmnd_list[0]);
            for(int i = 1; i < j; i++)
            {
                strcat(inp, " ");
                strcat(inp, cmnd_list[i]);
            }
            strcat(inp, " ");
            strcat(inp, buf);
        }
        else
            strcpy(inp, buf);
        printf("%s", inp);
        free(tempt);
        free(buf);
        for(int i = 0; i < count; i++)
            free(all_files[i]);
        closedir(d);
    }
    return (pos - pos_i);
}