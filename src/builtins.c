#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int shell_exit();
int shell_echo(environment_var *env);
int shell_type(environment_var *env);

int (*builtin_functions[]) (environment_var *env) = {
    &shell_exit,
    &shell_echo,
    &shell_type
};

int execute_builtin(environment_var *env, int id) {
    return (*builtin_functions[id]) (env);
}

int shell_exit() {
    return 0;
}

int shell_echo(environment_var *env) {
    for (int i = 1; env->args[i] != NULL; i++) {
        if (i > 1)
            printf(" ");
        printf("%s", env->args[i]);
    }
    printf("\n");

    return 1;
}

int shell_type(environment_var *env) {
    if (env->args[0] == NULL)
        return 1;

    for (int i = 0; builtin_names[i] != NULL; i++) {
        if (!strcmp(builtin_names[i], env->args[1])) {
            printf("%s is a shell builtin\n", env->args[1]);
            return 1;
        }
    }

    printf("%s: not found\n", env->args[1]);

    return 1;
}