#include "signals.h"

static int nrDirectories = 0;
static int nrFiles = 0;
int flag = 0;

/**
 * @brief Handler to deal with signals
 * 
 * @param signo Signal to be handled
 * 
*/
void sigusr_handler(int signo)
{
    if (signo == SIGUSR1)
    {
        nrDirectories++;
        printf("New directory: %d/%d directories/files at this time\n", nrDirectories, nrFiles);
    }
    else if (signo == SIGUSR2)
    {
        nrFiles++;
    }
    else if (signo == SIGINT)
    {
        flag = 1;
    }
}

/**
 * @brief Prepares signals and installs handlers
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int preparingSignal()
{
    struct sigaction action;
    // prepare the 'sigaction struct'
    action.sa_handler = sigusr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &action, NULL))
    {
        perror("preparingSignal SIGUSR1");
        return 1;
    }

    if (sigaction(SIGUSR2, &action, NULL))
    {
        perror("preparingSignal SIGUSR1");
        return 2;
    }

    if (sigaction(SIGINT, &action, NULL))
    {
        perror("preparingSignal SIGUSR1");
        return 3;
    }

    return 0;
}

/**
 * @brief Raises signals to the process with pid
 * 
 * @param msg Enum specifying signal to be treated
 *        pid Pid of the process to receive the signal
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int sendSignal(enum sig msg, int pid)
{
    switch (msg)
    {
    case 0:
        if (kill(pid, SIGUSR1))
        {
            perror("sendSignal");
            return 1;
        }
        break;

    case 1:
        if (kill(pid, SIGUSR2))
        {
            perror("sendSignal");
            return 1;
        }
        break;
    }

    return 0;
}