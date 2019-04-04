#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WhatToShow.h"

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>


int main(int argc, char *argv[], char *envp[])
{
    setbuf(stdout, NULL);

    WhatToShow whatToShow;

    if (initializeWhatToShowUser(&whatToShow, argv, argc))
    {
        return 1;
    }

    if (gettingOutput(whatToShow))
    {
        return 2;
    }

    return 0;
}