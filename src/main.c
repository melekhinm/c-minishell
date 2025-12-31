#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "shell.h"

int main() {
    int status;
    do {
        char *line = readline("$ ");
        char **args = parse_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}