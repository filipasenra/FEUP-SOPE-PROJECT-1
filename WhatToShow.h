#ifndef WhatToShow_H_
#define WhatToShow_H_

#include "log.h"
#include "output.h"
#include "signals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
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
    FILE *outputRegFile;
    bool saidaPadrao;
    char *outputFile;
    FILE *redOutputFile;
    char *file;
    bool is_file;
    clock_t start;

} WhatToShow;

/**
 * @brief Initializes struct WhatToShow given by argument with default values
 * 
 * @param whatToShow Struct WhatToShow to be initialized
*/
void constructorWhatToShow(WhatToShow *whatToShow);

/**
 * @brief Initializes struct WhatToShow with the arguments given by the user
 * 
 * @param whatToShow Struct
 *        argv Arguments
 *        argc Number of arguments
*/
int initializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc);

/**
 * @brief Displays the information accordingly with WhatToShow
 * 
 * @param whatToShow Struct
 * 
 * @return Returns 0 upon sucess, non-zero otherwise
*/
int gettingOutput(WhatToShow whatToShow);

/**
 * @brief Redirects Output if necessary
 * 
 * @param whatToShow Struct 
 * 
 * @return Return zero upon sucess, non-zero otherwise
*/
int redirectOutput(WhatToShow whatToShow);

/**
 * @brief Reedirects the output of the command given to a FILE
 * 
 * @param File to be analysed
 *        Command to be given
 * 
 * @return Returns a pointer to the FILE where the output of the command is redirected
*/
FILE *file_of_command(char *file, const char command[]);

void gettingTokens(WhatToShow *whatToShow, char *argv[], int argc, const char s[2]);

/**
 * @brief Verifies all arguments inserted by the user
 * 
 * @return Returns 0 in case of valid, right ordered arguments and non-zero otherwise
*/
int verifyInvalidArgInserts(char *argv[], int argc);

int foundNewDirectory(WhatToShow whatToShow, char *directory, char isFirstDir);

void outputInicialCommand(char * argv, int argc, char command[]);

#endif 