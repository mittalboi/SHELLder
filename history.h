#ifndef __HIST_H__
#define __HIST_H__

void load_history(char* hist_path);
void edit_history(char* input);
void save_history(char* hist_path);
void exec_history();
#endif