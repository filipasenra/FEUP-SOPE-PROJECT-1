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

enum sig {
    usr1,
    usr2
};

void sigusr_handler(int signo);

int sendSignal(enum sig msg);

#endif