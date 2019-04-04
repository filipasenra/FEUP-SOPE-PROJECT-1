#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WhatToShow.h"

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>

/**
 * Things to do:
 * foundNewDirectory & gettingOutput too confusing
 * I think signals might have a problem 
 * 
*/

void handling_exit()
{
    fcloseall();
}

int main(int argc, char *argv[], char *envp[])
{
    atexit(handling_exit);

    //setbuf(stdout, NULL);

    WhatToShow whatToShow;

    if (initializeWhatToShowUser(&whatToShow, argv, argc))
    {
        perror("ERROR INITIALIZING!");
        return 1;
    }

    if (gettingOutput(whatToShow))
    {
        return 2;
    }

    return 0;
}