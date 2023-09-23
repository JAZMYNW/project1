#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "myshell_parser.h"






int main(int argc, char *argv[]){

char *prompt = "my_shell$ ";
char *userin;
char line[MAX_LINE_LENGTH];
struct pipeline* my_pipeline;
int fd = 0;

	
  while(1){
	if((argc > 1) && (strcmp(argv[1], "-n") == 0)){
		//no prompt	
	}else{
		printf("%s",prompt); //prompt
	}

		if((userin = fgets(line, MAX_LINE_LENGTH, stdin))==NULL)
			break; //ctrl-d
		else{
			my_pipeline = pipeline_build(userin);//calling  pipeline build
			struct pipeline_command *command = my_pipeline->commands;
			while(command){
				printf("\n");
				for (int j = 0; command->command_args[j] != NULL; j++) {
            		printf("%s ", command->command_args[j]);
        		}
				if(command->redirect_in_path!= NULL){
					printf("< %s",command->redirect_in_path);
				}
				if(command->redirect_out_path!= NULL){
					printf("> %s",command->redirect_out_path);
				}
				command = command->next;
			}
        printf("\n");
			pid_t child = fork();
             
                if (child == 0) { // Child process
                    // Use execvp to execute the command
                    if (execvp(my_pipeline->commands->command_args[0], my_pipeline->commands->command_args) == -1) {
                        perror("ERROR: Execution");
                        exit(EXIT_FAILURE);
                    }
                } else { // Parent process
                    // Wait for the child process to finish
                    int status;
                    waitpid(child, &status, 0);
                }
            }
          
        
			
        }
		return 0;
    }



