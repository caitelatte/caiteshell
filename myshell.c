#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.g>

#define MAX_LENGTH 1024
#define DELIMITERS " /n/t"

const char *search_dir[] = { "/bin", "/usr/bin", ""};

int main (int argc, char** argv)
{
    // define line
    char input[MAX_LENGTH];
    char *cmd;

    // infinite loop
    while (1) {
        printf("c: ");
        if (!fgets(input, MAX_LENGTH, stdin)) { break; }
        // input is now in input

        //parse line
        if ((cmd = strtok(input, DELIMITERS))) {
            printf("%s\n", cmd);
        }
    }
}