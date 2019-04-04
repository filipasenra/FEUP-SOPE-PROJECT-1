#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

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

int gettingRegFileCommand(FILE *regFile, long double start, char *cmd);

int gettingRegFileSignalOne(FILE *regFile, long double start);

int gettingRegFileSignalTwo(FILE *regFile, long double start);

int gettingRegFileAnalized(char *file, FILE *regFile, long double start);

int gettingRegFileFinished(FILE *regFile, long double start);

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
int addLog(long double start, long double end, char act[], FILE *file_output);

long double gettingTime();

#endif
