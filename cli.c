#include "head.h"
#include "cli.h"

void print_cli(char* user, char* host, char* cwd)
{
    //printf("%d\n", duration);
    if(duration < 1)
        printf(CYN"%s@%s"MAG":"CYN"%s"MAG">> "RESET, user, host, cwd);
    else
        printf(CYN"%s@%s"MAG":"CYN"%s "YEL"[%ds]"MAG">> "RESET, user, host, cwd, duration);
}
