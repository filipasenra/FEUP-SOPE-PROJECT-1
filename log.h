#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

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
 * @brief Gets the current time
 * 
 * @return Returns a Struct timeval
*/
struct timeval gettingTime();

/**
 * @brief Outputs the log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 *        cmd Command given by the user
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileCommand(FILE *regFile, struct timeval start, char *cmd);

/**
 * @brief Outputs the SIGNAL USR1 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalOne(FILE *regFile, struct timeval start);

/**
 * @brief Outputs the SIGNAL USR2 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalTwo(FILE *regFile, struct timeval start);

/**
 * @brief Outputs the ANALIZED FILE log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileAnalized(char *file, FILE *regFile, struct timeval start);

/**
 * @brief Outputs the FINISHED PROCESS EXECUTION  log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileFinished(FILE *regFile, struct timeval start);

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
int addLog(struct timeval start, struct timeval end, char act[], FILE *file_output);

#endif
