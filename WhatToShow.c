#include "WhatToShow.h"

static int pid_pai;

void getInicialCommand(int argc, char *argv[], char command[])
{
    strcpy(command, argv[0]);

    for (int i = 1; i < argc; i++)
    {
        strcat(command, " ");
        strcat(command, argv[i]);
    }
}

int foundNewDirectory(WhatToShow whatToShow, char *directory, char isFirstDir)
{
    DIR *d;
    struct dirent *dir;

    d = opendir(directory);

    //Error opening directory
    if (!d)
    {
        printf("Failed to open %s directory!\n", directory);
        return 3;
    }

    //Makes the currents directory the one passed by the user
    if (!whatToShow.is_file)
        chdir(directory);

    //Found directory, sending SIGUSR1
    if (!whatToShow.saidaPadrao)
    {
        enum sig msg = usr1;
        sendSignal(msg, pid_pai);

        //Print to log file case necessary
        if (whatToShow.registosExecucao)
        {
            if (gettingRegFileSignalOne(whatToShow.outputRegFile, whatToShow.start))
                printf("Failed getting log file");
        }
    }

    while ((dir = readdir(d)) != NULL)
    {
        if (flag)
        {
            kill(0, SIGKILL);
            exit(0);
        }

        bool dir_current_old = strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..");

        //If it is a file or a if it isn't to analyse subfolders
        if (dir->d_type == DT_REG || (!whatToShow.analiseAll && dir_current_old))
        {
            if (!isFirstDir)
                fprintf(whatToShow.redOutputFile, "%s/", directory);

            //Found file, sending SIGUSR2
            if (!whatToShow.saidaPadrao)
            {
                enum sig msg = usr2;
                sendSignal(msg, pid_pai);

                //Print to log file case necessary
                if (whatToShow.registosExecucao)
                {
                    if (gettingRegFileSignalTwo(whatToShow.outputRegFile, whatToShow.start))
                        printf("Failed getting log file");
                }
            }

            if (gettingOutputFile(dir->d_name, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256, whatToShow.redOutputFile))
                return -1;

            //Print to log file case necessary
            if (whatToShow.registosExecucao)
            {
                if (gettingRegFileAnalized(dir->d_name, whatToShow.outputRegFile, whatToShow.start))
                    printf("Failed getting log file");
            }
        }
        else if (dir->d_type == DT_DIR && dir_current_old) //If it is a directory
        {
            int pid = fork();

            if (pid == 0) //child working
            {
                if (foundNewDirectory(whatToShow, dir->d_name, FALSE))
                    return -1;

                sleep(20);

                closedir(d);

                exit(0);
            }
            else if (pid > 0) //father working
            {

                while (wait(NULL))
                {
                    if (errno == EINTR)
                        continue;
                    else
                        break;
                }
            }
        }
    }
    closedir(d);

    return 0;
}

void gettingTokens(WhatToShow *whatToShow, char *argv[], int argc, const char s[2])
{
    char *token;

    /* get the first token */
    token = strtok(argv[argc + 1], s);

    /* walk through other tokens */
    while (token != NULL)
    {
        if (strcmp(token, "md5") == 0)
        {
            whatToShow->MD5 = true;
        }
        else if (strcmp(token, "sha1") == 0)
            whatToShow->SHA1 = true;
        else if (strcmp(token, "sha256") == 0)
            whatToShow->SHA256 = true;

        token = strtok(NULL, s);
    }
}

/**
 * @brief Verifies all arguments inserted by the user
 * 
 * @param argv Arguments
 *        argc Number of arguments 
 * 
 * @return Returns 0 in case of valid, right ordered arguments and non-zero otherwise
*/
int verifyInvalidArgInserts(char *argv[], int argc)
{

    //invalid number of arguments
    if (argc < 2 || argc > 8)
    {
        printf("Use: %s [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>", argv[0]);
        return 1;
    }

    //if argument given is a file, no possible -r argument
    struct stat path_stat;
    if (stat(argv[argc - 1], &path_stat) < 0)
    {
        printf("FileStat failed!\n");
        return 2;
    }

    //check for user argument insertion failure on -r
    if (S_ISREG(path_stat.st_mode))
    {
        if (strcmp(argv[1], "-r") == 0)
        {
            printf("Invalid arguments! No '-r' argument needed for a file");
            return 3;
        }
    }

    //check order
    int order = 1;
    int ordered = 0;
    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], "-r") == 0)
            order = 1;
        else if (strcmp(argv[i], "-h") == 0)
            order = 2;
        else if (strcmp(argv[i], "-o") == 0)
            order = 4;
        else if (strcmp(argv[i], "-v") == 0)
            order = 6;
        else if (order == 2 || order == 4)
        {
            order++;
        }

        if (order <= ordered)
            return 4;
        ordered = order;
    }

    return 0;
}

/**
 * @brief Initializes struct WhatToShow given by argument with default values
 * 
 * @param whatToShow Struct WhatToShow to be initialized
*/
void constructorWhatToShow(WhatToShow *whatToShow)
{
    whatToShow->start = times(NULL);
    whatToShow->analiseAll = false;
    whatToShow->MD5 = false;
    whatToShow->SHA1 = false;
    whatToShow->SHA256 = false;
    whatToShow->registosExecucao = false;
    whatToShow->outputRegExe = NULL;
    whatToShow->outputRegFile = NULL;
    whatToShow->saidaPadrao = true;
    whatToShow->outputFile = NULL;
    whatToShow->redOutputFile = stdout;
    whatToShow->file = NULL;
    whatToShow->is_file = false;
}

int is_file(WhatToShow *whatToShow)
{
    //Check if file is a diretory or a file
    struct stat path_stat;

    if (stat(whatToShow->file, &path_stat) < 0)
    {
        printf("FileStat failed!\n");
        return 1;
    }

    if (S_ISREG(path_stat.st_mode))
        whatToShow->is_file = TRUE;

    return 0;
}

int inicializeRegistosExe(WhatToShow *whatToShow, char *argv[], int argc)
{
    whatToShow->registosExecucao = true;
    whatToShow->outputRegExe = getenv("LOGFILENAME");

    if (whatToShow->outputRegExe == NULL)
    {
        char tmp[] = "logoutput.txt";
        whatToShow->outputRegExe = tmp;
    }

    //Opening log file
    whatToShow->outputRegFile = fopen(whatToShow->outputRegExe, "a");

    //String with all args given
    char command[256] = "";
    getInicialCommand(argc, argv, command);

    if (gettingRegFileCommand(whatToShow->outputRegFile, whatToShow->start, command))
        printf("Failed getting log file");

    return 0;
}

/**
 * @brief Initializes struct WhatToShow with the arguments given by the user
 * 
 * @param whatToShow Struct
 *        argv Arguments
 *        argc Number of arguments
*/
int initializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc)
{
    //check for eventual user errors
    if (verifyInvalidArgInserts(argv, argc))
    {
        return 1;
    }

    //initialize struct with default values
    constructorWhatToShow(whatToShow);

    //let's do it backwards
    argc--;

    //saving file or directory to analise
    whatToShow->file = argv[argc];

    //Check if file is a diretory or a file
    is_file(whatToShow);

    //handling other arguments representing functionalities
    for (int i = argc - 1; i > 0; i--)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            inicializeRegistosExe(whatToShow, argv, argc);
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            whatToShow->saidaPadrao = false;
            whatToShow->outputFile = argv[i + 1];
            whatToShow->redOutputFile = fopen(whatToShow->outputFile, "w");
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            gettingTokens(whatToShow, argv, i, ",");
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            whatToShow->analiseAll = true;
        }
    }

    return 0;
}

/**
 * @brief Displays the information accordingly with WhatToShow
 * 
 * @param whatToShow Struct
 * 
 * @return Returns 0 upon sucess, non-zero otherwise
*/
int gettingOutput(WhatToShow whatToShow)
{
    preparingSignal();

    pid_pai = getpid();

    //If it is a file and not a (sym)link or a directory
    if (whatToShow.is_file)
    {
        //Found file, sending SIGUSR2
        if (!whatToShow.saidaPadrao)
        {
            enum sig msg = usr2;
            sendSignal(msg, pid_pai);

            //Print to log file case necessary
            if (whatToShow.registosExecucao)
            {
                if (gettingRegFileSignalTwo(whatToShow.outputRegFile, whatToShow.start))
                    printf("Failed getting log file");
            }
        }

        if (gettingOutputFile(whatToShow.file, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256, whatToShow.redOutputFile))
            printf("Failed getting output file");

        //Print to log file case necessary
        if (whatToShow.registosExecucao)
        {
            if (gettingRegFileAnalized(whatToShow.file, whatToShow.outputRegFile, whatToShow.start))
                printf("Failed getting log file");
        }
    }
    //If it is not a file, lets show the information of all files
    else
    {
        if (foundNewDirectory(whatToShow, whatToShow.file, TRUE))
        {
            printf("Failed finding new directory %s", whatToShow.file);
            return 1;
        }
    }

    //Rights in the console if necessary
    if (!whatToShow.saidaPadrao)
    {
        printf("Data saved on file %s\n", whatToShow.outputFile);
        fclose(whatToShow.redOutputFile);
    }

    //Closing log file if necessary
    if (whatToShow.registosExecucao)
    {
        if (gettingRegFileFinished(whatToShow.outputRegFile, whatToShow.start))
            printf("Failed getting log file");

        fclose(whatToShow.outputRegFile);
    }

    return 0;
}