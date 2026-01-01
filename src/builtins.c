#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int shell_exit();
int shell_echo(char **args);
int shell_type(char **args);

int (*builtin_functions[]) (char **) = {
    &shell_exit,
    &shell_echo,
    &shell_type
};

int execute_builtin(char **args, int id) {
    return (*builtin_functions[id]) (args);
}

int shell_exit() {
    return 0;
}

int shell_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        if (i > 1)
            printf(" ");
        printf("%s", args[i]);
    }
    printf("\n");

    return 1;
}

int shell_type(char **args) {
    if (args[1] == NULL)
        return 1;

    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(builtin_names[i], args[1])) {
            printf("%s is a shell builtin\n", args[1]);
            return 1;
        }
    }

    printf("%s: not found\n", args[1]);

    return 1;
}