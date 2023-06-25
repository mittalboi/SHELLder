#include "history.h"
#include "head.h"


void load_history(char* hist_path)
{
    fp = fopen(hist_path, "r+");
    int i = 0;
    char* line = NULL;
    size_t len = 0;
    while((getline(&line, &len, fp)) != -1) 
    {
        if(i >= 20)
        {
            for(int j = 0; j < 19; j++)
            {
                strcpy(histarry[j],histarry[j+1]);
            }
            strcpy(histarry[19], line);       //histarry[19] holds latest command
        }
        else
        {
            strcpy(histarry[i], line);
            i++;
        }
    }
    hix = i;
}

void edit_history(char* input)
{
    if(hix >= 20)         //code for replacing oldest command
    {
        for(int j = 0; j < 19; j++)
        {
            strcpy(histarry[j],histarry[j+1]);
        }
        strcpy(histarry[19], input);       //histarry[19] holds latest command
    }
    else
    {
        strcpy(histarry[hix], input);
        hix++;
    }
    return;
}

void save_history(char* hist_path)
{
    fp = freopen(hist_path, "w", fp);
    if(fp == NULL)
    {
        perror("history.txt!");
        exit(1);
    }
    for(int i = 0; i < hix; i++)
        fprintf(fp,"%s", histarry[i]);
    return;
}

void exec_history()
{
    int i;
    if(hix - 10 > 0)
        i = hix - 10; 
    else
        i = 0;
    for(; i < hix; i++)
    {
        printf("%s", histarry[i]);
    }
    return;
}