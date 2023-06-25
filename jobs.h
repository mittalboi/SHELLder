#ifndef __JOBS_H__
#define __JOBS_H__

#include <unistd.h>

struct Job {
    char* name;
    int jno;
    pid_t pid;
    int state; 
};

void save_job(char *args_list[], int args_cnt, pid_t pid);
void exec_jobs();
void sort_jobs();
struct Job get_job(pid_t pid);
int get_pid_by_jno(int jno);

#endif