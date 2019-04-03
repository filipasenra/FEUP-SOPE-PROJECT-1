#include "log.h"

int gettingRegFileCommand(FILE *regFile, clock_t start, char *cmd)
{
    char act[256] = "COMMAND ";
    strcat(act, cmd);

    //Adding register to the log file
    if (addLog(start, times(NULL), act, regFile))
    {
        printf("Failed printing to log file\n");
        return 1;
    }

    return 0;
}

int gettingRegFileSignalOne(FILE *regFile, clock_t start)
{
    // Setting up char array act
    char act[256] = "SIGNAL USR1";

    //Adding register to the log file
    if (addLog(start, times(NULL), act, regFile))
    {
        printf("Failed printing to log file\n");
        return 1;
    }

    return 0;
}

int gettingRegFileSignalTwo(FILE *regFile, clock_t start)
{
    //Setting up char array act
    char act[256] = "SIGNAL USR2";

    //Adding register to the log file
    if (addLog(start, times(NULL), act, regFile))
    {
        printf("Failed printing to log file\n");
        return 1;
    }

    return 0;
}

int gettingRegFileAnalized(char *file, FILE *regFile, clock_t start)
{
    //Setting up char array act
    char act[256] = "ANALIZED ";
    strcat(act, file);

    //Adding register to the log file
    if (addLog(start, times(NULL), act, regFile))
    {
        printf("Failed printing to log file\n");
        return 1;
    }

    return 0;
}

int gettingRegFileFinished(FILE *regFile, clock_t start)
{
    //Setting up char array act
    char act[256] = "Finished process execution";

    //Adding register to the log file
    if (addLog(start, times(NULL), act, regFile))
    {
        printf("Failed printing to log file\n");
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
int addLog(clock_t start, clock_t end, char act[], FILE *file_output)
{
    long ticks = sysconf(_SC_CLK_TCK);
    double inst = ((double)end - start) / ticks * 1000;
    fprintf(file_output, "%4.2f", inst);
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