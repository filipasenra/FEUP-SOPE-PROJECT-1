#include "WhatToShow.h"

void gettingTokens(WhatToShow *whatToShow, char *argv[], int argc, const char s[2])
{
    char *token;

    /* get the first token */
    token = strtok(argv[argc + 1], s);

    /* walk through other tokens */
    while (token != NULL)
    {
        if (strcmp(argv[argc], "md5"))
            whatToShow->MD5 = true;
        else if (strcmp(argv[argc], "sha1"))
            whatToShow->SHA1 = true;
        else if (strcmp(argv[argc], "sha256"))
            whatToShow->SHA256 = true;

        token = strtok(NULL, s);
    }
}

//forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>

void inicializeWhatToShow(WhatToShow *whatToShow)
{
    whatToShow->analiseAll = false;
    whatToShow->MD5 = false;
    whatToShow->SHA1 = false;
    whatToShow->SHA256 = false;
    whatToShow->registosExecucao = false;
    whatToShow->outputRegExe = NULL;
    whatToShow->saidaPadrao = true;
    whatToShow->outputFile = NULL;
    whatToShow->file = NULL;
}

/**
 * @brief Inicializes struct WhatToShow with the arguments given by the user
*/
void inicializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc)
{
    if (argc < 2 || argc > 8)
    {
        printf("Use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
        return;
    }

    inicializeWhatToShow(whatToShow);

    //let's start from the back
    argc--;

    //saving file or directory to analise
    whatToShow->file = argv[argc];
    argc--;

    if (argc < 1)
        return;

    while (argc >= 1)
    {
        if (strcmp(argv[argc], "-v") == 0)
        {
            whatToShow->registosExecucao = true;
            whatToShow->outputRegExe = getenv("LOGFILENAME");
        }
        else if (strcmp(argv[argc], "-o") == 0)
        {
            whatToShow->saidaPadrao = false;
            whatToShow->outputFile = argv[argc + 1];
        }
        else if (strcmp(argv[argc], "-h") == 0)
        {
            gettingTokens(whatToShow, argv, argc, ",");
        }
        else if (strcmp(argv[argc], "-r") == 0)
        {
            whatToShow->analiseAll = true;
        }

        argc--;
    }
}

/**
 * @Brief Displays the information accordingly with WhatToShow
*/
void gettingOutput(WhatToShow whatToShow)
{
    struct stat path_stat;
    if (stat(whatToShow.file, &path_stat) < 0)
    {
        printf("FileStat failed!\n");
        return;
    }

    //If it is a file and not a (sym)link or a directory
    if (S_ISREG(path_stat.st_mode))
    {
        gettingOutputFile(whatToShow.file, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256);
        return;
    }

    //If it is not a file, lets show the information of all files
    DIR *d;
    struct dirent *dir;

    //Makes the currents directory the one passed by the user
    d = opendir(whatToShow.file);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //If it is a file and not a (sym)link or a directory
            if (dir->d_type == DT_REG)
            {
                gettingOutputFile(whatToShow.file, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256);
            }
        }
        closedir(d);
    }
    else
    {
        printf("Failed to open %s directory!\n", whatToShow.file);
        return;
    }
}

/**
 * @brief Reedirects the output of the command given to a FILE
 * 
 * @param File to be analysed
 * @param Command to be given
 * 
 * @return Returns a pointer to the FILE where the output of the command is redirected
*/
FILE *file_of_command(char *file, const char command[])
{
    FILE *in = NULL;
    char cmd[256];
    strcpy(cmd, command);
    strcat(cmd, file);
    in = popen(cmd, "r");

    return in;
}

/**
 * @brief Getting the output of a file
 * 
 * @param name of the file
 * @param if it should display MD5 hash
 * @param if it should display SHA1 hash
 * @param if it should display SHA256 hash
 * 
*/
void gettingOutputFile(char *file, bool MD5, bool SHA1, bool SHA256)
{
    struct stat fileStat;

    if (stat(file, &fileStat) < 0)
    {
        printf("FileStat failed!\n");
        return;
    }

    //===========================================
    //FILE NAME
    printf("Name of file: %s\n", file);

    //===============================================
    //TYPE OF FILE

    //Runs command of sheel, but to read within program
    FILE *in_type_of_file = file_of_command(file, "file ");

    //Reads line by line the result of the command
    char temp[256];
    fgets(temp, 255, in_type_of_file);

    //Cuts C-string to give only what we want
    char *type_file = strndup(temp + strlen(file) + 2, strlen(temp));

    printf("Type of file: %s", type_file);

    //===============================================
    //FILE SIZE
    printf("File Size: \t\t%d bytes\n", fileStat.st_size);

    //===============================================
    //FILE PERMISSIONS - TO BE MODIFIED
    printf("File Permissions: \t");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    //================================================
    //MODIFICATION TIME

    //Extracting modification time in the struct tm
    struct tm *modification_time;
    modification_time = localtime(&fileStat.st_mtime);

    //Printing modification time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d\n", modification_time->tm_mday, modification_time->tm_mon + 1, modification_time->tm_year + 1900, modification_time->tm_hour, modification_time->tm_min, modification_time->tm_sec);

    //================================================
    //LAST ACESS TIME

    //Extracting last acess time in the struct tm
    struct tm *last_acess_time;
    last_acess_time = localtime(&fileStat.st_atime);

    //Printing last acess time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d\n", last_acess_time->tm_mday, last_acess_time->tm_mon + 1, last_acess_time->tm_year + 1900, last_acess_time->tm_hour, last_acess_time->tm_min, last_acess_time->tm_sec);

    //=================================================
    //HASH

    if (MD5)
    {
        FILE *in_MD5 = file_of_command(file, "md5sum ");

        //Reads line by line the result of the command
        fgets(temp, 255, in_MD5);

        //Cuts C-string to get only what we want
        char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 1));

        printf("MD5: %s\n", string_md5);
    }

    if (SHA1)
    {
        FILE *in_MD5 = file_of_command(file, "sha1sum ");

        //Reads line by line the result of the command
        fgets(temp, 255, in_MD5);

        //Cuts C-string to get only what we want
        char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 1));

        printf("sha1: %s\n", string_md5);
    }

    if (SHA256)
    {
        FILE *in_MD5 = file_of_command(file, "sha256sum ");

        //Reads line by line the result of the command
        fgets(temp, 255, in_MD5);

        //Cuts C-string to get only what we want
        char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 1));

        printf("sha256: %s\n", string_md5);
    }

    //=================================================
}