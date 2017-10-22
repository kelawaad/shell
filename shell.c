#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"


int main(int argc, char** argv) {
    int is_running = 1;
    char *current_command = malloc((COMMAND_MAXLEN + 1) * sizeof(char)); // The current command being executed
    file_name = getFileName(__FILE__);
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

            debug_print("Command: %s\n", current_command);
            debug_print("Command length: %d\n", command_length);

            if(strcmp(current_command, "exit") == 0)
                break;


            char **command_params = parseCommand(current_command, command_length, &num_args);
            if(num_args > 0 && command_params[num_args - 1][0] == '&')
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
