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

void gettingOutput(char * file)
{
    struct stat fileStat;

    if (stat(file, &fileStat) < 0)
    {
        printf("FileStat failed!\n");
        return;
    }

    //file name
    printf("Name of file: %s\n", file);

    //file size
    printf("File Size: \t\t%d bytes\n",fileStat.st_size);

    //File Permissions
    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    //Extracting modification time in the struct tm
    struct tm * modification_time;
    modification_time = localtime(&fileStat.st_mtime);

    //Printing modification time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d\n", modification_time->tm_mday, modification_time->tm_mon + 1, modification_time->tm_year + 1900, modification_time->tm_hour, modification_time->tm_min, modification_time->tm_sec);

    //Extracting modification time in the struct tm
    struct tm * last_acess_time;
    last_acess_time = localtime(&fileStat.st_atime);

    //Printing modification time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d\n", last_acess_time->tm_mday, last_acess_time->tm_mon + 1, last_acess_time->tm_year + 1900, last_acess_time->tm_hour, last_acess_time->tm_min, last_acess_time->tm_sec);

}