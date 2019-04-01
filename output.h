#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

int outputTypeOfFile(char file[]);
int outputTimeISO_8601 (struct tm * time);
int outputHash(char file[], char command[]);
int outputPermissions(mode_t mode);

#endif