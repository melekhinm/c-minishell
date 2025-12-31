#include <stdio.h>
#include <stdlib.h>

int shell_exit();
int shell_echo(char **args);

int (*builtin_functions[]) (char **) = {
    &shell_exit,
    &shell_echo
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