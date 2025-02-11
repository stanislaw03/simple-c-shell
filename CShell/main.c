#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/wait.h>

#define TRUE 1

char command[256];
char *parameters[256];

void type_prompt() {
    printf("[####]$");                            //Literally just a prompt to simulate a shell
}

void read_command(char *command, char **parameters) {   //String tokeniser, gets input and tokenises it to seperate the command
    char input[1024];                                   //and its parameters
    fgets(input, sizeof(input), stdin);

    input[strcspn (input, "\n")] = 0;

    char *token = strtok (input, " ");

    int i;
    for(i = 0; token != NULL; i++) {
        parameters[i] = token;
        token = strtok(NULL, " ");
    }
    parameters[i] = NULL;

    strcpy(command, parameters[0]);
}

int main(int argc, char *argv[]) {      //Main with forks and inbuilt cd command, will not work outside of linux shell. 
    while (TRUE) {
        type_prompt();
        read_command(command, parameters);

        if(strcmp(command, "cd") == 0) {
            if(parameters[1] != NULL) {
                if(chdir(parameters[1]) == -1){
                    perror("cd command failed");
                    continue;
                }
            }else {
                chdir(getenv("HOME"));
            }
            continue;
        }
        if(strcmp(command, "exit") == 0) {
            printf("Quitting program\n");
            break;
        }
        pid_t pid = fork();
        if(pid < 0) {
            perror("fork failed");
            exit(1);
        }
        else if(pid > 0) {
            waitpid(pid, &status, 0);
        }
        else {
            char command_path[1024] = "/usr/bin/";
            strcat(command_path, command);
            if(execv(command_path, command_path) == -1) {
                perror("execv failed");
                exit(1);
            }
            return 0;
        }
    }
}