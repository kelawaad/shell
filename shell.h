#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG
// Uncomment the next line to stop debugging output
//#undef DEBUG

#ifdef DEBUG
    #define DEBUG_TEST 1
#else
    #define DEBUG_TEST 0
#endif // DEBUG

#define debug_print(fmt, ...) \
        do { if (DEBUG_TEST) fprintf(stderr, "%s:%d:%s():"fmt, file_name, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#ifdef _WIN32
    #define PATH_SEPARATOR '\\'
#elif __linux__
    #define PATH_SEPARATOR '/'
#endif

// Maximum length of a command;
#define COMMAND_MAXLEN 512
// Maximum number of arguments per command
#define MAX_ARGS_NUM 100



char *file_name;

char **parseCommand(char *command, int command_length, int *num_args) {
    if(command == NULL)
        return NULL;

    debug_print("%s, %d\n", command, command_length);

    char *current_str = malloc((COMMAND_MAXLEN + 1) * sizeof(char));
    char **parsed_command = malloc(MAX_ARGS_NUM * sizeof(char*));
    int index = 0, current_str_index = 0, command_index = 0;
    while(index < command_length)
    {
        current_str_index = 0;
        while(index < command_length && command[index] == ' ')
            index++;
        while(index < command_length && command[index] != ' ' && command[index] != 0)
            current_str[current_str_index++] = command[index++];
        current_str[current_str_index] = 0;

        parsed_command[command_index] = malloc((current_str_index + 1) * sizeof(char));
        strcpy(parsed_command[command_index++], current_str);

        debug_print("\tindex = %d\tcommand_length = %d\tcurrent_str_index=%d\n", \
            index, command_length, current_str_index);
        debug_print("\tcurrent_str=%s\tparsed_command[%d]=%s\n\n",\
            current_str, command_index - 1, \
            parsed_command[command_index - 1]);
    }
    parsed_command[command_index] = NULL;
    *num_args = command_index;
    free(current_str);
    return parsed_command;
}

void printArgs(char **args)
{
    int i = 0;
    while(args[i] != NULL)
        printf("%s\n", args[i++]);
}

char* getFileName(char *file_name_wpath)
{
    int length = strlen(file_name_wpath);
    int index = length - 1;
    while(file_name_wpath[index] != PATH_SEPARATOR)
        index--;

    char *file_name = malloc((length - index + 1) * sizeof(char));
    int i = index + 1, j = 0;

    // Copy the filename into the filename variable
    for(;i < length;)
        file_name[j++] = file_name_wpath[i++];

    file_name[j] = 0; // Add '\0' to the end of the string
    return file_name;
}


int getCommandLength(char *command) {
    if(command == NULL)
        return -1;

    int length = 0;
    while(command[length] != 0)
    {
        length++;
        if(length > COMMAND_MAXLEN)
            return 0; // Return 0 if the command exceeds the maximum limit
    }
    return length;
}



#endif // SHELL_H_INCLUDED
