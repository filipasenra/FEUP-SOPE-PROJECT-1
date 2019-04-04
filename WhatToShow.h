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
#include <errno.h>

#define FALSE 0
#define TRUE 1

extern int flag;

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
    struct timeval start;

} WhatToShow;

/**
 * @brief Gets the initial command given by the user
 * 
 * @param argc Number of arguments in argv
 *        argv Arguments given by the user
 *        command String that reseives the command
 * 
*/
void getInitialCommand(int argc, char *argv[], char command[]);

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
 * @brief Updates whatToShow with the hashes requested by the user
 * 
 * @param whatToShow Struct
 *        argument C-string with the format 'md5,sha1,sha256'
 *        s Comma
 * 
*/
void gettingTokens(WhatToShow *whatToShow, char * argument, const char s[2]);

/**
 * @brief Verifies all arguments inserted by the user
 * 
 * @param argv Arguments
 *        argc Number of arguments 
 * 
 * @return Returns 0 in case of valid, right ordered arguments and non-zero otherwise
*/
int verifyInvalidArgInserts(char *argv[], int argc);

/**
 * @brief Deals with new directory 
 * 
 * @param whatToShow Struct
 *        directory Directory to be analised
 *        isFirstDir signals if it is the first directory (the directory passed by the user)
 * 
 * @return Returns 0 upon sucess, non-zero otherwise
*/
int foundNewDirectory(WhatToShow whatToShow, char *directory, char isFirstDir);

/**
 * @brief Determinates if the user chose a file or a directory
 * 
 * @param whatToShow Struct
 * 
 * @return Returns 0 upon sucess, non-zero otherwise
*/
int is_file(WhatToShow *whatToShow);

/**
 * @brief Updates whatToShow when the user chose to have Execution Registers
 * 
 * @param whatToShow Struct
 *        argv
 *        argc
 * 
 * @return Returns 0 upon sucess, non-zero otherwise
*/
int initializeRegistosExe(WhatToShow *whatToShow, char *argv[], int argc);

#endif 