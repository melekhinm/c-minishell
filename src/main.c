#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "shell.h"

int shell_execute(char **args);

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

int shell_execute(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        fprintf(stderr, "%s\n", args[i]);
    }

    return 1;
}
