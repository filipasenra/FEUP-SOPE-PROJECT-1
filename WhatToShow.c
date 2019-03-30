#include "WhatToShow.h"

int foundNewDirectory(WhatToShow whatToShow, char *directory, const char path[])
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
        //If it is a file and not a (sym)link or a directory
        if (dir->d_type == DT_REG)
        {
            //Prints the path of the file
            printf("%s", path);

            if (gettingOutputFile(dir->d_name, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256))
                return -1;
        }
        else if (dir->d_type == DT_DIR && whatToShow.analiseAll) //If it is a directory
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
            {
                int pid = fork();

                if (pid == 0) //child working
                {
                    char tmp_path[256];
                    strcpy(tmp_path, path);

                    //If the path is not empty, add a '/'
                    if (strcmp(tmp_path, ""))
                        strcat(tmp_path, "/");

                    strcat(tmp_path, dir->d_name);
                    strcat(tmp_path, "/");

                    if (foundNewDirectory(whatToShow, dir->d_name, tmp_path))
                        return -1;

                    //When it has finished working the directory, it returns
                    break;
                }
                else if (pid > 0) //father working
                {
                    //Waist for child to finished
                    wait(NULL);
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
void initializeWhatToShow(WhatToShow *whatToShow)
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
 * @brief Initializes struct WhatToShow with the arguments given by the user
 * 
 * @param whatToShow Struct
 *        argv Arguments
 *        argc Number of arguments
*/
void initializeWhatToShowUser(WhatToShow *whatToShow, char *argv[], int argc)
{
    //check for eventual user errors
    if (verifyInvalidArgInserts(argv, argc))
    {
        return;
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

        dup2(file1, 1);
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
            gettingOutputFile(whatToShow.file, whatToShow.MD5, whatToShow.SHA1, whatToShow.SHA256);
            return 0;
        }
        //If it is not a file, lets show the information of all files
        else
        {
            if (foundNewDirectory(whatToShow, whatToShow.file, ""))
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

    return 0;
}

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
    strcat(cmd, file);
    in = popen(cmd, "r");

    return in;
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
int gettingOutputFile(char *file, bool MD5, bool SHA1, bool SHA256)
{
    struct stat fileStat;

    if (stat(file, &fileStat) < 0)
    {
        printf("FileStat failed!\n");
        return 1;
    }

    //===========================================
    //FILE NAME
    printf("%s, ", file);

    //===============================================
    //TYPE OF FILE

    //Runs command of shell, but to read within program
    FILE *in_type_of_file = file_of_command(file, "file ");

    if (in_type_of_file == NULL)
    {
        printf("Error in file_of_command ");
        return 2;
    }

    //Reads line by line the result of the command
    char temp[256];
    fgets(temp, 255, in_type_of_file);

    //Cuts C-string to give only what we want
    char *type_file = strndup(temp + strlen(file) + 2, strlen(temp));

    for (int i = 0; i < strlen(type_file); i++)
    {
        if (type_file[i] == '\n')
        {
            type_file[i] = 0;
            break;
        }
    }

    printf("%s, ", type_file);

    pclose(in_type_of_file);

    //===============================================
    //FILE SIZE
    printf("%ld, ", fileStat.st_size);

    //===============================================
    //FILE PERMISSIONS - TO BE MODIFIED
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    //printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    //printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    //printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    //printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    //printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    //printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(", ");

    //================================================
    //MODIFICATION TIME

    //Extracting modification time in the struct tm
    struct tm *modification_time;
    modification_time = localtime(&fileStat.st_mtime);

    //Printing modification time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d, ", modification_time->tm_mday, modification_time->tm_mon + 1, modification_time->tm_year + 1900, modification_time->tm_hour, modification_time->tm_min, modification_time->tm_sec);

    //================================================
    //LAST ACESS TIME

    //Extracting last acess time in the struct tm
    struct tm *last_acess_time;
    last_acess_time = localtime(&fileStat.st_atime);

    //Printing last acess time in ISO 8601 (<date>T<time>) format
    printf("%d-%d-%dT%d-%d-%d", last_acess_time->tm_mday, last_acess_time->tm_mon + 1, last_acess_time->tm_year + 1900, last_acess_time->tm_hour, last_acess_time->tm_min, last_acess_time->tm_sec);

    //=================================================
    //HASH

    if (MD5)
    {
        FILE *in_MD5 = file_of_command(file, "md5sum ");

        if (in_type_of_file == NULL)
        {
            printf("Error in file_of_command!\n");
            return 3;
        }

        //Reads line by line the result of the command
        if (fgets(temp, 255, in_MD5) == NULL)
        {
            printf("Error in fgets!\n");
            return 4;
        }

        //Cuts C-string to get only what we want
        char *string_md5 = strndup(temp, strlen(temp) - (strlen(file) + 3));

        printf(", %s", string_md5);

        pclose(in_MD5);
    }

    if (SHA1)
    {
        FILE *in_sha1 = file_of_command(file, "sha1sum ");

        if (in_sha1 == NULL)
        {
            printf("Error in file_of_command!\n");
            return 5;
        }

        //Reads line by line the result of the command
        if (fgets(temp, 255, in_sha1) == NULL)
        {
            printf("Error in fgets!\n");
            return 6;
        }

        //Cuts C-string to get only what we want
        char *string_sha1 = strndup(temp, strlen(temp) - (strlen(file) + 3));

        printf(", %s", string_sha1);

        pclose(in_sha1);
    }

    if (SHA256)
    {
        FILE *in_sha256 = file_of_command(file, "sha256sum ");

        if (in_sha256 == NULL)
        {
            printf("Error in file_of_command!\n");
            return 7;
        }

        //Reads line by line the result of the command
        if (fgets(temp, 255, in_sha256) == NULL)
        {
            printf("Error in fgets!\n");
            return 8;
        }

        //Cuts C-string to get only what we want
        char *string_sha256 = strndup(temp, strlen(temp) - (strlen(file) + 3));
        printf(", %s", string_sha256);

        pclose(in_sha256);
    }

    printf("\n");

    //=================================================

    return 0;
}
