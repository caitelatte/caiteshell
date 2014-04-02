/* 
 * written by Caitlin Macleod
 * Licensed under GPL version 3
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LENGTH 1023
#define DELIMITERS "\n "

const char *search_dir[] = { "/bin", "/usr/bin", ""};
int is_running;
const char *prompt = "caitec:";

static int linehandler (char *line);

typedef struct {
	char input_str[MAX_LENGTH];
	char *cmd;
	char **argv;
	int argc;
}COMMAND;

/* linehandler takes in the raw input line that is entered by the user
 * and handles 
 * */
static int linehandler (char *line)
{
	COMMAND *input_ptr;
	input_ptr = malloc(sizeof(COMMAND));
	char **save_ptr;
	save_ptr = (char **)malloc(sizeof(char));
	int exit_status;
    input_ptr->argc=0;
    input_ptr->argv=calloc(255, sizeof(char*));
	if ((input_ptr == 0)||(save_ptr==0)||(input_ptr->argv==0)) {
		// not enough space for mallocing these things.
		printf("ERROR: not enough memory\n");
		exit_status = EXIT_FAILURE;
	}
	
	// store line in input_ptr->input_str so that I can replace chars with null using strtok while keeping original line.
	strcpy(input_ptr->input_str, line);
	input_ptr->argv[input_ptr->argc]=input_ptr->input_str;
    
	// parse args and add their pointers to input->argv, with first arg being stored in input->cmd
    if ((input_ptr->argv[input_ptr->argc] = strtok_r(input_ptr->input_str, DELIMITERS, save_ptr))) {
		// get first part of line, set to cmd.
		input_ptr->cmd=input_ptr->argv[input_ptr->argc];
		input_ptr->argc++;
	}
	while ((input_ptr->argv[input_ptr->argc] = strtok_r(NULL, DELIMITERS, save_ptr))) {
		input_ptr->argc++;
	}
	free(save_ptr);
	
    // handle "exit", "help", and executing the command.
	if (!strcmp(input_ptr->cmd, "exit")) {
		is_running = 0;
		printf("Exiting caiteshell. :( See you next time.\n");
		
	} else if (!strcmp(input_ptr->cmd, "help")) {
		printf("Hello! Welcome to caiteshell.\nTo exit, enter \"exit\".\nTo run a program that's located within your current directory,\n/usr/bin or /bin, just enter the program's name.\n");
	} else {
		pid_t pid;
		int status;
		switch (pid=fork()) {
			case -1: // error in the forking :(
				printf("We had an error starting the process. Please try again.\n");
				exit_status = EXIT_FAILURE;
				break;
			case 0: // hello i am a child
				status = execvp(input_ptr->cmd, input_ptr->argv);
				exit (status);
				break;
			default: // ooh we are a parent
				// wait for thing to finish 
				if(waitpid(pid, &status, 0) < 0) {
					printf("We're sorry, there was an error with the process you tried to run.\n");
					exit_status = EXIT_FAILURE;
				}
				// wifexited should return true if the child process exited normally.
				if (WIFEXITED(status)) { 
                    printf ("An error occured in the process you specified.\nType and submit \"help\" if you need a hand.\n");
					exit_status = WEXITSTATUS(status);
					break;
				}
				exit_status = EXIT_FAILURE;
				break;
		}
	}
	free(input_ptr->argv);
	free(input_ptr);
	return(exit_status);
}

int main (int argc, char** argv)
{
    // define line handler
    char raw_input[MAX_LENGTH];
	is_running = 1;

    // infinite loop
    while (is_running) {
		// take in input
        printf("c: ");
        if (!fgets(raw_input, MAX_LENGTH, stdin)) { 
			is_running = 0; 
			printf("See ya!\n");
		}
		if (is_running) { linehandler(raw_input); }
    }
	exit(EXIT_SUCCESS);
}
