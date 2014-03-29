#include <stdio.h>
// #include <readline/readline.h>
// #include <readline/history.h>
#include <stdlib.h>

#define MAX_LENGTH 1024
#define DELIMITERS " /n/t|"

const char *search_dir[] = { "/bin", "/usr/bin", ""};
int is_running;
const char *prompt = "caitec:";

static void cb_linehandler (char *line);
static char *current_path ();

static void cb_linehandler (char *line)
{
    // handle exit
    // search for executable
    //  if found: fork and wait for executable
    //  otherwise: printf("Sorry, \"%s\" couldn't be found in %s, usr/bin/ or /bin/.", cmd, current_path());
}

static char *current_path () { }

int main (int argc, char** argv)
{
    // define line handler
    char input[MAX_LENGTH];
    char *cmd;
    is_running = 1;

    // install 
    //rl_callback_handler_install(PROMPT, cb_linehandler);

    // infinite loop
    while (is_running) {
        printf("c: ");
        if (!fgets(input, MAX_LENGTH, stdin)) { break; }
        // input is now in input

        //parse line
        if ((cmd = strtok(input, DELIMITERS))) {
        	// get first part of line, set to cmd.
            printf("%s\n", cmd);
            printf("%s\n", input);
            
        }
    }
}