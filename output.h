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

/**
 * @brief Outputs the type of a file
 * 
 * @param file File to be analysed
 *        outputFile Pointer to a file where the output should be written
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTypeOfFile(char file[], FILE * outputFile);

/**
 * @brief Outputs the given date and time in the format ISO 8601
 * 
 * @param tm Struct with date and time
 *        outputFile Pointer to a file where the output should be written
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTimeISO_8601 (struct tm * time, FILE * outputFile);

/**
 * @brief Outputs an hash
 * 
 * @param file Name of the file to be analized
 *        command Command declaring which hash to be outputed
 *        outputFile Pointer to a file where the output should be written
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputHash(char file[], char command[], FILE * outputFile);

/**
 * @brief Outputs files' permissions
 * 
 * @param mode Bit mask of the modes
 *        outputFile Pointer to a file where the output should be written
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
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

/**
 * @brief Reedirects the output of the command given to a FILE
 * 
 * @param File to be analysed
 *        Command to be given
 * 
 * @return Returns a pointer to the FILE where the output of the command is redirected
*/
FILE *file_of_command(char *file, const char command[]);

#endif