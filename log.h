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

/**
 * @brief Gives the current time in miliseconds
 * 
 * @return Returns the current time in miliseconds
*/
long double gettingTime();

/**
 * @brief Outputs the log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 *        cmd Command given by the user
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileCommand(FILE *regFile, long double start, char *cmd);

/**
 * @brief Outputs the SIGNAL USR1 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalOne(FILE *regFile, long double start);

/**
 * @brief Outputs the SIGNAL USR2 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalTwo(FILE *regFile, long double start);

/**
 * @brief Outputs the ANALIZED FILE log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileAnalized(char *file, FILE *regFile, long double start);

/**
 * @brief Outputs the FINISHED PROCESS EXECUTION  log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileFinished(FILE *regFile, long double start);

/**
* @brief Adds a log at the end of a file
*
* @param start  Initial instant
*        end    Final instant
*        act    Description
*        file_output Output file's name
*
* @return Return zero upon sucess, non-zero otherwise
*/
int addLog(long double start, long double end, char act[], FILE *file_output);

long double gettingTime();

#endif
