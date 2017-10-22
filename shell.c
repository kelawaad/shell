#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Maximum length of a command;
#define COMMAND_MAXLEN 512
// Maximum number of arguments per command
#define MAX_ARGS_NUM 100

#define DEBUG
//#undef DEBUG

int getCommandLength(char *command);
char** parseCommand(char *command, int command_length, int *num_args);
void printArgs(char **args);


int main(int argc, char** argv) {
    int is_running = 1;
    char *current_command = malloc((COMMAND_MAXLEN + 1) * sizeof(char)); // The current command being executed
    while(is_running) {

        printf("Shell>");
        fgets(current_command, COMMAND_MAXLEN + 1, stdin);

        int command_length = getCommandLength(current_command);
        int run_in_background = 0;
        int num_args;

        if(command_length != 0)
        {
            // Remove the '\n' from the command
            current_command[command_length - 1] = 0;
            command_length--;

            #ifdef DEBUG
            printf("Command: %s\n", current_command);
            printf("Command length: %d\n", command_length);
            #endif // DEBUG

            if(strcmp(current_command, "exit") == 0)
                break;


            char **command_params = parseCommand(current_command, command_length, &num_args);
            if(command_params[num_args - 1][0] == '&')
                run_in_background = 1;
            /*
            pid_t pid = fork();
            int status;
            if(pid == 0)
            {
                //CHILD Process
                int error = execvp(command_params[0], command_params);

            } else if (pid > 0) {
                // PARENT Process
                if(run_in_background) {

                } else {

                }
            }
            */
        }
    } //end while
    return 0;
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



char **parseCommand(char *command, int command_length, int *num_args) {
    if(command == NULL)
        return NULL;

    #ifdef DEBUG
        printf("parseCommand(%s, %d)\n", command, command_length);
    #endif // DEBUG

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

        #ifdef DEBUG
            printf("\tindex = %d\tcommand_length = %d\tcurrent_str_index=%d\n",index, command_length, current_str_index);
            printf("\tcurrent_str=%s\tparsed_command[%d]=%s\n\n", current_str, command_index - 1, parsed_command[command_index - 1]);
        #endif // DEBUG
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


