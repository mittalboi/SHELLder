#ifndef __TABS_H__
#define __TABS_H__

int autocomplete(char *inp, char* username, char* hostname, char* ncwd, char* home_dir);
void die(const char *s);
void disableRawMode();
void enableRawMode();
#endif