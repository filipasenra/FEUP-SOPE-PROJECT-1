#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

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



int gettingRegFile(char *file, FILE *regFile, time_t start, enum act description, char *cmd);


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
int addLog(time_t start, time_t end, char act[], FILE *file_output);

#endif
