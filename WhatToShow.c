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

        } else if (strcmp(argv[argc], "-o") == 0)
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