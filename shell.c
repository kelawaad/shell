#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include "shell.h"

// Uncomment the next line stop debugging output
#undef DEBUG
#define DEBUG 0

int main(int argc, char** argv) {
	//printf("%s", getenv("PATH"));
    int is_running = 1;
    char *current_command = malloc((COMMAND_MAXLEN + 1) * sizeof(char)); // The current command being executed
    file_name = getFileName(__FILE__);
    while(is_running) {

        printf("Shell>");
        fgets(current_command, COMMAND_MAXLEN + 1, stdin);

        int command_length = getCommandLength(current_command);
		if(command_length == COMMAND_MAX_LEN_EXCEEDED)
		{
			printf("Command max limit exceeded\n");
		}
        int run_in_background = 0;
        int num_args;

        if(command_length != 0)
        {
            // Remove the '\n' from the command
            current_command[command_length - 1] = 0;
            command_length--;

            debug_print("Command: %s\n", current_command);
            debug_print("Command length: %d\n", command_length);

            if(strcmp(current_command, "exit") == 0)
			{
				is_running = 0;
                break;
			}

            char **command_params = parseCommand(current_command, command_length, &num_args);
            if(num_args > 0 && command_params[num_args - 1][0] == '&')
			{
				command_params[num_args - 1] = NULL;
                run_in_background = 1;
            }

			if(strcmp(command_params[0], "cd") == 0)
			{
				printf("CD command\n");
				printf("PWD = %s\n", getenv("PWD"));
				//int ret = chdir(command_params[1]);
				if(ret == -1)
                    print_error("cd");
				continue;
			}

            pid_t pid = fork();
            int status;
            if(pid == 0)
            {
                //CHILD Process
                int error = execvp(command_params[0], command_params);
				print_error(command_params[0]);
				break;

            } else if (pid > 0) {
                // PARENT Process

                if(run_in_background) {
					// NOT WORKING correctly
					waitpid(pid, &status, WNOHANG);
                } else {
					wait(&status);
                }
            }
        }
    } //end while
	return 0;
}

char **parseCommand(char *command, int command_length, int *num_args) {
    if(command == NULL)
        return NULL;

    debug_print("%s, %d\n", command, command_length);

    char *current_str = malloc((command_length + 1) * sizeof(char));
    char **parsed_command = malloc(MAX_ARGS_NUM * sizeof(char*));
    int index = 0, current_str_index = 0, command_index = 0;
    while(index < command_length)
    {
        current_str_index = 0;
        while(index < command_length && command[index] == ' ')
            index++;
		if(index >= command_length)
			break;
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


// Prints a NULL-terminated list of strings
void printArgs(char **args)
{
    int i = 0;
    while(args[i] != NULL)
        printf("%s\n", args[i++]);
}


// Gets the name of the source file
char* getFileName(char *file_name_wpath)
{
    int length = strlen(file_name_wpath);
    int index = length - 1;
    while(index >= 0 && file_name_wpath[index] != PATH_SEPARATOR)
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
            return COMMAND_MAX_LEN_EXCEEDED; // Return 0 if the command exceeds the maximum limit
    }
    return length;
}

void print_error(char *command) {
	int err = errno;
	debug_print("Error = %d", err);
	if(strcmp(command, "cd") == 0) {
        if(err == ENOENT) {
            // Directory doesn't exist
        }
        else if(err == ENOTDIR) {
            // A component is not a directory
        }
        else if(err == EFAULT) {

        }
        else if(err == EACCES) {

        }
	}
	if(err == ENOENT)
	{
		printf("%s: command not found\n", command);
	}

}
