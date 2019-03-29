#ifndef WhatToShow_H_
#define WhatToShow_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <dirent.h> 
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

#define FALSE 0
#define TRUE 1

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

/**
 * @brief Initializes struct WhatToShow given by argument with default values
 * 
 * @param whatToShow Struct WhatToShow to be initialized
*/
void initializeWhatToShow(WhatToShow *whatToShow);

/**
 * @brief Initializes struct WhatToShow with the arguments given by the user
 * 
 * @param whatToShow Struct
 *        argv Arguments
 *        argc Number of arguments
*/
void initializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc);

int gettingOutput(WhatToShow whatToShow);

int gettingOutputFile(char * file, bool MD5, bool SHA1, bool SHA256);

/**
 * @brief Verifies all arguments inserted by the user
 * 
 * @return Returns 0 in case of valid, right ordered arguments and non-zero otherwise
*/
int verifyInvalidArgInserts(char *argv[], int argc);

int foundNewDirectory(WhatToShow whatToShow, char *directory, char isFirstDir);

#endif 