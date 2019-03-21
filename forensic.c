#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WhatToShow.h"

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>

int main(int argc, char *argv[], char *envp[])
{
    WhatToShow whatToShow;

    inicializeWhatToShowUser(&whatToShow, argv, argc);

    printf("%s\n", whatToShow.file);

    printf("%s\n", whatToShow.outputFile);

    return 0;
}