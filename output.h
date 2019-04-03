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

int outputTypeOfFile(char file[], FILE * outputFile);
int outputTimeISO_8601 (struct tm * time, FILE * outputFile);
int outputHash(char file[], char command[], FILE * outputFile);
int outputPermissions(mode_t mode, FILE * outputFile);

/**
 * @brief Getting the output of a file
 * 
 * @param file Name of the file
 *        MD5 If it should display MD5 hash
 *        SHA1 If it should display SHA1 hash
 *        SHA256 If it should display SHA256 hash
 * 
 * @return Returns zero upon sucess, non-zero otherwise
*/
int gettingOutputFile(char * file, bool MD5, bool SHA1, bool SHA256, FILE * outputFile);

#endif