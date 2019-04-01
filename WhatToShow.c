#include "WhatToShow.h"

/**
 * @brief Constructs a string with the initial command given by the user
 * 
 * @param whatToShow Struct with the command arguments
 *        path_stat Struct 
 * 
 * @return Return the string with the initial command
 */
void initialCommand(WhatToShow whatToShow, bool folder, char command[])
{
    strcpy(command, "forensic");

    if (whatToShow.analiseAll)
        strcat(command, " -r");

    if (whatToShow.MD5 || whatToShow.SHA1 || whatToShow.SHA256)
        strcat(command, " -h");

    if (whatToShow.MD5)
        strcat(command, " md5");

    if (whatToShow.SHA1)
        strcat(command, ",sha1");

    if (whatToShow.SHA256)
        strcat(command, ",sha256");

    if (!whatToShow.saidaPadrao)
    {
        strcat(command, " -o ");
        strcat(command, whatToShow.outputFile);
    }

    if (whatToShow.registosExecucao)
        strcat(command, " -v ");

    //If it is a folder, print ./
    if (folder)
        strcat(command, "./");

    strcat(command, whatToShow.file);
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
    chdir(directory);

    while ((dir = readdir(d)) != NULL)
    {
        //If it is a file or a if it isn't to analyse subfolders
        if (dir->d_type == DT_REG || (!whatToShow.analiseAll && strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")))
        {
            if (!isFirstDir)
                printf("%s/", directory);

            if (gettingOutputFile(dir->d_name, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256))
                return -1;

            //Print to log file case necessary
            if (whatToShow.registosExecucao)
            {
                enum act description = analized;
                if (gettingRegFile(dir->d_name, whatToShow.outputRegFile, whatToShow.start, description, NULL))
                    printf("Failed getting log file");
            }
        }
        else if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) //If it is a directory
        {
            int pid = fork();

            if (pid == 0) //child working
            {
                if (foundNewDirectory(whatToShow, dir->d_name, FALSE))
                    return -1;

                break;
            }
            else if (pid > 0) //father working
            {
                wait(NULL);
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
void initializeWhatToShow(WhatToShow *whatToShow)
{
    whatToShow->start = time(NULL);
    whatToShow->analiseAll = false;
    whatToShow->MD5 = false;
    whatToShow->SHA1 = false;
    whatToShow->SHA256 = false;
    whatToShow->registosExecucao = false;
    whatToShow->outputRegExe = NULL;
    whatToShow->outputRegFile = NULL;
    whatToShow->saidaPadrao = true;
    whatToShow->outputFile = NULL;
    whatToShow->file = NULL;
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
    initializeWhatToShow(whatToShow);

    //let's do it backwards
    argc--;

    //saving file or directory to analise
    whatToShow->file = argv[argc];

    //continuing backwards
    argc--;

    //handling other arguments representing functionalities
    while (argc > 0)
    {
        if (strcmp(argv[argc], "-v") == 0)
        {
            whatToShow->registosExecucao = true;
            whatToShow->outputRegExe = getenv("LOGFILENAME");

            if (whatToShow->outputRegExe == NULL)
            {
                char tmp[] = "logfile.txt";
                whatToShow->outputRegExe = tmp;
            }
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

        //continuing backwards
        argc--;
    }

    return 0;
}

/**
 * @brief Redirects Output if necessary
 * 
 * @param whatToShow Struct 
 * 
 * @return Return zero upon sucess, non-zero otherwise
*/
int redirectOutput(WhatToShow whatToShow)
{

    if (!whatToShow.saidaPadrao)
    {
        //opens file to reedirect output
        int file1 = open(whatToShow.outputFile, O_WRONLY | O_TRUNC);
        if (file1 == -1)
        {
            perror("ERROR OPENING DESTINATION FILE!");
            return 1;
        }

        dup2(file1, STDOUT_FILENO);
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
    struct stat path_stat;
    if (stat(whatToShow.file, &path_stat) < 0)
    {
        printf("FileStat failed!\n");
        return 1;
    }

    //String with all args given
    char cmd[256];

    if (S_ISREG(path_stat.st_mode))
        initialCommand(whatToShow, false, cmd);
    else
        initialCommand(whatToShow, true, cmd);

    //Printing first execution - program initialization
    if (whatToShow.registosExecucao)
    {
        //Opening log file
        whatToShow.outputRegFile = fopen(whatToShow.outputRegExe, "a");

        enum act description = command;
        if (gettingRegFile(whatToShow.file, whatToShow.outputRegFile, whatToShow.start, description, cmd))
            printf("Failed getting log file");
    }

    //Child can mess with reedirecting printf to the file
    int pid = fork();

    if (pid == 0) /* child */
    {
        //Reedirect Output to File Given by User if necessary
        if (redirectOutput(whatToShow) != 0)
            return 2;

        //If it is a file and not a (sym)link or a directory
        if (S_ISREG(path_stat.st_mode))
        {
            if (gettingOutputFile(whatToShow.file, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256))
                printf("Failed getting output file");

            //Print to log file case necessary
            if (whatToShow.registosExecucao)
            {
                enum act description = analized;
                if (gettingRegFile(whatToShow.file, whatToShow.outputRegFile, whatToShow.start, description, NULL))
                    printf("Failed getting log file");
            }

            return 0;
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
    }
    else if (pid > 0) /* father */
    {
        //waits for child
        wait(NULL);

        //Rights in the console if necessary
        if (!whatToShow.saidaPadrao)
        {
            printf("Data saved on file %s\n", whatToShow.outputFile);
        }
    }
    else
    {
        printf("ERROR in creating fork!\n");
        return 4;
    }

    //Closing log file if necessary
    if (whatToShow.registosExecucao)
    {
        enum act description = finished;
        if (gettingRegFile(whatToShow.file, whatToShow.outputRegFile, whatToShow.start, description, NULL))
            printf("Failed getting log file");
        fclose(whatToShow.outputRegFile);
    }

    return 0;
}