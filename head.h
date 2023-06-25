#ifndef __HEAD_H__
#define __HEAD_H__

#include "cli.h"
#include "commandcmp.h"
#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "history.h"
#include "pinfo.h"
#include "discover.h"
#include "jobs.h"
#include "sig.h"
#include "fbg.h"
#include "tabs.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <ctype.h>

//All are bold
#define RED "\x1B[31;1m"
#define GRN "\x1B[32;1m"
#define YEL "\x1B[33;1m"
#define BLU "\x1B[34;1m"
#define MAG "\x1B[35;1m"
#define CYN "\x1B[36;1m"
#define WHT "\x1B[37;1m"
#define RESET "\x1B[0m"

#define MAX_JOBS 1000

extern char* pre_wd;
extern time_t duration;
extern FILE* fp;
extern char* histarry[20];
extern int hix;
extern int p[2];
extern struct Job jlist[MAX_JOBS];
extern struct Job temp_jlist[MAX_JOBS];
extern int jsize, jmax;
extern char* curr_proc;
extern pid_t par_pid;
extern pid_t curr_pid;
#endif