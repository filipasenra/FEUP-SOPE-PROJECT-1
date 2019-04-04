#include "log.h"

/**
 * @brief Gives the current time in miliseconds
 * 
 * @return Returns the current time in miliseconds
*/
long double gettingTime() {
    long double ms; // Milliseconds
    struct timespec spec;

    clock_gettime(CLOCK_MONOTONIC, &spec);

    ms = (long double) spec.tv_nsec / 1.0e4; // Convert nanoseconds to milliseconds
    ms = ms/100;
    
    return ms;
}

/**
 * @brief Outputs the command log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 *        cmd Command given by the user
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileCommand(FILE *regFile, long double start, char *cmd)
{
    char act[256] = "COMMAND ";
    strcat(act, cmd);

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileCommand\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Outputs the SIGNAL USR1 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalOne(FILE *regFile, long double start)
{
    // Setting up char array act
    char act[256] = "SIGNAL USR1";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileSignalOne\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Outputs the SIGNAL USR2 log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileSignalTwo(FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "SIGNAL USR2";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileSignalTwo\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Outputs the ANALIZED FILE log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileAnalized(char *file, FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "ANALIZED ";
    strcat(act, file);

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileAnalized\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Outputs the FINISHED PROCESS EXECUTION  log to the Register File
 * 
 * @param regFile Pointer to a file to be written with the logs
 *        start Start time of execution
 * 
 * @return Returns 0 upon success, non-zero otherwise
*/
int gettingRegFileFinished(FILE *regFile, long double start)
{
    //Setting up char array act
    char act[256] = "Finished process execution";

    //Adding register to the log file
    if (addLog(start, gettingTime(), act, regFile))
    {
        printf("Failed printing to log file in gettingRegFileFinished\n");
        return 1;
    }

    return 0;
}

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
int addLog(long double start, long double end, char act[], FILE *file_output)
{
    long double inst = end-start;

    fprintf(file_output, "%.2Lf", inst);
    fflush(file_output);

    fprintf(file_output, " - ");
    fflush(file_output);

    fprintf(file_output, "%d", getpid());
    fflush(file_output);
    fprintf(file_output, " - ");
    fflush(file_output);

    fprintf(file_output, "%s", act);
    fflush(file_output);
    fprintf(file_output, "\n");
    fflush(file_output);

    return 0;
}