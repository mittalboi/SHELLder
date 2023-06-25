#ifndef __LS_H__
#define __LS_H__

void exec_ls(char* args_list[], int args_cnt, char* home_dir);
void ls_la(char* dir_file, char* home_dir, int t);
void ls_l(char* dir_file, char* home_dir, int t);
void ls_a(char* dir_file, char* home_dir, int t);
void ls_fd(char* dir_file, char* home_dir, int t);
#endif