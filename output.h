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
 * @brief Gets the output of the type of a file
 * 
 * @param file File to be analysed
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTypeOfFile(char file[], char output[]);

/**
 * @brief Gets the output of the given date and time in the format ISO 8601
 * 
 * @param tm Struct with date and time
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTimeISO_8601 (struct tm * time, char output[]);

/**
 * @brief Gets the outputs of an hash
 * 
 * @param file Name of the file to be analized
 *        command Command declaring which hash to be outputed
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputHash(char file[], char command[], char output[]);

/**
 * @brief Gets the outputs of the files' permissions
 * 
 * @param mode Bit mask of the modes
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputPermissions(mode_t mode, char output[]);

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