#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WhatToShow.h"

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>

/**
 * Things to do:
 * foundNewDirectory & gettingOutput too confusing
 * I think the inicial command can be simplified if it is done when we have acess to argc and argv * 
 * Repiting Data saved on file 
 * 
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