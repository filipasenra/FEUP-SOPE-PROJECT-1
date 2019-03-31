#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WhatToShow.h"

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>

/**
 * Things to do:
 * 1. getting the paramether from the user must be more robust
 * 2. Ouput is still incomplete
 *      2.1. Output function should return a C-string or a struct with the values 
 *      to be printed in the console or the output file - resolved, i think?
 *      2.2. The file permissions to be displayed are from whom?
 * 3.Brief description for gettingTokens() and Filipa how it works?
 * 4.gettingOutputFile -> printfs()? and file permissions?
*/

int main(int argc, char *argv[], char *envp[])
{
    setbuf(stdout, NULL);

    WhatToShow whatToShow;

    if (initializeWhatToShowUser(&whatToShow, argv, argc)) {
        perror("ERROR INITIALIZING!");
        return 1;
    }
    
    if (gettingOutput(whatToShow)) {
        perror("ERROR GETTING OUTPUT!");
        return 2;
    }

    return 0;
}