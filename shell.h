#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 1

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s():"fmt, file_name, \
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

#define NULL_COMMAND -1
#define COMMAND_MAX_LEN_EXCEEDED -2

char *file_name;

char **parseCommand(char *command, int command_length, int *num_args);
void printArgs(char **args);
char* getFileName(char *file_name_wpath);
int getCommandLength(char *command);
void print_error(char *command);

#endif // SHELL_H_INCLUDED
