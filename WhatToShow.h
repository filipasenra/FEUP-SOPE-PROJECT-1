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
    FILE *outputRegFile;
    bool saidaPadrao;
    char *outputFile;
    char *file;
    clock_t start;

} WhatToShow;

/**
 * @brief Enum to define which act is to be printed on the log file
*/
enum act {
    command,
    signalOne,
    signalTwo,
    analized,
    finished
};

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

/**
 * @brief Getting the output of a file
 * 
 * @param file Name of the file
 *        MD5 If it should display MD5 hash
 *        SHA1 If it should display SHA1 hash
 *        SHA256 If it should display SHA256 hash
 * 
 * @return Returns zero upon sucess, non-zero otherwise
*/
int gettingOutputFile(char * file, bool MD5, bool SHA1, bool SHA256);

void gettingTokens(WhatToShow *whatToShow, char *argv[], int argc, const char s[2]);

/**
 * @brief Verifies all arguments inserted by the user
 * 
 * @return Returns 0 in case of valid, right ordered arguments and non-zero otherwise
*/
int verifyInvalidArgInserts(char *argv[], int argc);

int foundNewDirectory(WhatToShow whatToShow, char *directory, char isFirstDir);

/**
* @brief Adds a log at the end of a file
*
* @param start  Initial instant
*        end    Final instant
*        act    Description
*        output Output file's name
*
* @return Return zero upon sucess, non-zero otherwise
*/
int addLog(clock_t start, clock_t end, char act[], FILE *file_output);

void initialCommand(WhatToShow whatToShow, struct stat path_stat, char *command);

int gettingRegFile(char *file, FILE *regFile, clock_t start, enum act description, char *cmd);

#endif 