#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1023
#define DELIMITERS "\n "

const char *search_dir[] = { "/bin", "/usr/bin", ""};
int is_running;
const char *prompt = "caitec:";

static void linehandler (char *line);

typedef struct {
	char input_str[MAX_LENGTH];
	char *cmd;
	char **argv;
	int argc;
}COMMAND;

/* linehandler takes in the raw input line that is entered by the user
 * and handles 
 * */
static void linehandler (char *line)
{
	COMMAND *input_ptr;
	input_ptr = malloc(sizeof(COMMAND));
	char **save_ptr;
	save_ptr = (char **)malloc(sizeof(char));
	pid_t pid;
	if ((input_ptr == 0)||(save_ptr==0)) {
		// not enough space for input
		printf("ERROR: not enough memory");
		exit(EXIT_FAILURE);
	}
	strcpy(input_ptr->input_str, line);
	input_ptr->argc=0;
	input_ptr->argv=calloc(255, sizeof(char*));
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
	
    // handle exit, help, and executing a thing.
	if (!strcmp(input_ptr->cmd, "exit")) {
		is_running = 0;
		printf("Exiting caiteshell. :( See you next time.\n");
	} else if (!strcmp(input_ptr->cmd, "help")){
		printf("Hello! Welcome to caiteshell.\nTo exit, enter \"exit\".\nTo run a program that's located within your current\ndirectory, usr/bin or /bin, just enter the program's name.\n");
	} else {
		//  if found: fork and wait for executable
		switch (pid=fork()) {
			case -1:
				printf("");
				exit(EXIT_FAILURE);
			case 0: // hello i am a child
				status = execve(input_ptr->cmd, input_ptr->argv, input_ptr->argc);
				break;
			default: // ooh we are a parent
				break;
		}
		//  otherwise: 
		printf("\"%s\" couldn't be found in your current directory, usr/bin/ or /bin/.\n", input_ptr->cmd);
	}
	free(input_ptr->argv);
	free(input_ptr);
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
			printf("See ya!");
		}
		if (is_running) { linehandler(raw_input); }
    }
	exit(EXIT_SUCCESS);
}
