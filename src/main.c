#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "shell.h"

int main() {
    int status;

    environment_var *env = malloc(sizeof(environment_var));
    if (env == NULL) {
        fprintf(stderr, "shell: Could not start\n");
        exit(1);
    }

    env->home_dir = NULL;
    env->path_dirs = NULL;

    do {
        env->line = readline("$ ");
        parse_line(env);
        status = shell_execute(env);

        free(env->line);
        free(env->args);
    } while (status);
}