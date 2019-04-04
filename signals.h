#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/**
 * @brief enum to identify signals
*/
enum sig {
    usr1,
    usr2
};

/**
 * @brief Handler to deal with signals
 * 
 * @param signo Signal to be handled
 * 
*/
void sigusr_handler(int signo);

/**
 * @brief Prepares signals and installs handlers
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int preparingSignal();

/**
 * @brief Raises signals to the process with pid
 * 
 * @param msg Enum specifying signal to be treated
 *        pid Pid of the process to receive the signal
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int sendSignal(enum sig msg, int pid);

#endif