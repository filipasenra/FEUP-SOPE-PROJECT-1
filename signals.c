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
    else if (signo == SIGUSR2)
    {
        nrFiles++;
    }
    else if (signo == SIGINT)
    {
        
    }
}

int preparingSignal()
{
    struct sigaction action;
    // prepare the 'sigaction struct'
    action.sa_handler = sigusr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    return 0;
}

int sendSignal(enum sig msg, int pid)
{
    // install the handler
    switch (msg)
    {
    case 0:
        if (kill(pid, SIGUSR1))
        {
            printf("Failed to raise SIGUSR1!\n");
            return 1;
        }
        break;

    case 1:
        if (kill(pid, SIGUSR2))
        {
            printf("Failed to raise SIGUSR2!\n");
            return 1;
        }
        break;
    }

    return 0;
}