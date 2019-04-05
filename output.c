#include "output.h"

/**
 * @brief Reedirects the output of the command given to a FILE
 * 
 * @param File to be analysed
 *        Command to be given
 * 
 * @return Returns a pointer to the FILE where the output of the command is redirected
*/
FILE *file_of_command(char *file, const char command[])
{
    FILE *in = NULL;
    char cmd[256];
    strcpy(cmd, command);
    strcat(cmd, " ");
    strcat(cmd, file);
    in = popen(cmd, "r");

    return in;
}

/**
 * @brief Gets the output of the type of a file
 * 
 * @param file File to be analysed
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTypeOfFile(char file[], char output[])
{
    //===============================================
    //TYPE OF FILE

    //Runs command of shell, but to read within program
    FILE *in_type_of_file = file_of_command(file, "file ");

    if (in_type_of_file == NULL)
    {
        perror("OutputTypeOfFile");
        return 2;
    }

    //Reads line by line the result of the command
    char temp[256];
    fgets(temp, 255, in_type_of_file);

    //Cuts C-string to give only what we want
    char *type_file = strndup(temp + strlen(file) + 2, strlen(temp));

    for (int i = 0; i < (int)strlen(type_file); i++)
    {
        if (type_file[i] == '\n')
        {
            type_file[i] = 0;
            break;
        }
    }

    strcat(output, type_file);
    strcat(output, ", ");

    pclose(in_type_of_file);

    return 0;
}

/**
 * @brief Gets the output of the given date and time in the format ISO 8601
 * 
 * @param tm Struct with date and time
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputTimeISO_8601(struct tm *time, char output[])
{
    char time_s[256];

    //Printing modification time in ISO 8601 (<date>T<time>) format
    sprintf(time_s, "%d-%d-%dT%d-%d-%d", time->tm_mday, time->tm_mon + 1, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec);

    strcat(output, time_s);

    return 0;
}

/**
 * @brief Gets the outputs of an hash
 * 
 * @param file Name of the file to be analized
 *        command Command declaring which hash to be outputed
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputHash(char file[], char command[], char output[])
{

    FILE *hash = file_of_command(file, command);

    if (hash == NULL)
    {
        perror("OutputHash");
        return 3;
    }

    //Reads line by line the result of the command
    char temp[256];

    if (fgets(temp, 255, hash) == NULL)
    {
        perror("outputHash");
        return 4;
    }

    //Cuts C-string to get only what we want
    char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 3));

    strcat(output, ", ");
    strcat(output, string_md5);

    pclose(hash);

    return 0;
}

/**
 * @brief Gets the outputs of the files' permissions
 * 
 * @param mode Bit mask of the modes
 *        output C-string that the output will be added at the end of it
 * 
 * @return Returns 0 upon success and non-zero otherwise
*/
int outputPermissions(mode_t mode, char output[])
{
    strcat(output, (mode & S_IWUSR) ? "w" : "-");
    strcat(output, (mode & S_IWGRP) ? "w" : "-");
    strcat(output, (mode & S_IWOTH) ? "w" : "-");
    strcat(output, ", ");

    return 0;
}

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
int gettingOutputFile(char *file, bool MD5, bool SHA1, bool SHA256, FILE *outputFile)
{
    struct stat fileStat;

    char output[256];

    if (stat(file, &fileStat) < 0)
    {
        printf("FileStat failed!\n");
        return 1;
    }

    //FILE NAME
    sprintf(output, "%s, ", file);

    //TYPE OF FILE
    outputTypeOfFile(file, output);

    //FILE SIZE
    char file_s[256];
    sprintf(file_s, "%ld, ", fileStat.st_size);
    strcat(output, file_s);

    //FILE PERMISSIONS
    outputPermissions(fileStat.st_mode, output);

    //MODIFICATION TIME
    outputTimeISO_8601(localtime(&fileStat.st_mtime), output);
    strcat(output, ", ");

    //LAST ACESS TIME
    outputTimeISO_8601(localtime(&fileStat.st_atime), output);

    //HASH

    //se é um diretorio, não tem hash
    if (S_ISDIR(fileStat.st_mode))
    {
        strcat(output, "\n");
        return 0;
    }

    if (MD5)
    {
        char md5command[] = "md5sum ";
        outputHash(file, md5command, output);
    }

    if (SHA1)
    {
        char sha1command[] = "sha1sum ";
        outputHash(file, sha1command, output);
    }

    if (SHA256)
    {
        char sha256command[] = "sha256sum ";
        outputHash(file, sha256command, output);
    }

    strcat(output, "\n");

    fprintf(outputFile, "%s", output);

    return 0;
}