#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <dirent.h> 
#pragma once

/**
 * @brief struct to save specifications given by the user
*/
typedef struct
{

    bool analiseAll;
    bool MD5;
    bool SHA1;
    bool SHA256;
    bool registosExecucao;
    char *outputRegExe;
    bool saidaPadrao;
    char *outputFile;
    char *file;

} WhatToShow;

void inicializeWhatToShow(WhatToShow *whatToShow);

void inicializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc);

void gettingOutput(WhatToShow whatToShow);

void gettingOutputFile(char * file, bool MD5, bool SHA1, bool SHA256);