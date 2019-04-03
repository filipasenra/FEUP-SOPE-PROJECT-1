#include "signals.h"

static int nrDirectories = 0;
static int nrFiles = 0;

void sigusr_handler(int signo)
{
    if (signo == SIGUSR1) 
    {
        nrDirectories++;
        printf("New directory: %d/%d directories/files at this time\n", nrDirectories, nrFiles);
    }
    if (signo == SIGUSR2)
    {
        nrFiles++;
    }
}

int sendSignal(enum sig msg)
{
    struct sigaction action;
    // prepare the 'sigaction struct'
    action.sa_handler = sigusr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    // install the handler
    switch (msg)
    {
    case 0:
        sigaction(SIGUSR1, &action, NULL);
        if (raise(SIGUSR1))
        {
            printf("Failed to raise SIGUSR1!");
            return 1;
        }
        break;

    case 1:
        sigaction(SIGUSR2, &action, NULL);
        if (raise(SIGUSR2))
        {
            printf("Failed to raise SIGUSR2!");
            return 1;
        }
        break;
    }

    return 0;
}