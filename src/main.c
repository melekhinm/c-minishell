#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "shell.h"

void free_env(environment_var *env);
void free_string_array(char **strings);

int main() {
    int status;

    environment_var *env = malloc(sizeof(environment_var));
    if (env == NULL) {
        fprintf(stderr, "shell: Could not start\n");
        exit(1);
    }

    env->full_path = NULL;

    do {
        env->home_dir = strdup(getenv("HOME"));
        env->line = readline("$ ");
        parse_line(env);
        parse_path(env);
        status = shell_execute(env);

        free_env(env);
    } while (status);

    free(env);
}

void free_env(environment_var *env) {
    if (env == NULL)
        return;

    if (env->args != NULL) {
        free_string_array(env->args);
        free(env->args);
        env->args = NULL;
    }

    if (env->line != NULL) {
        free(env->line);
        env->line = NULL;
    }

    if (env->home_dir != NULL) {
        free(env->home_dir);
        env->home_dir = NULL;
    }

    if (env->full_path != NULL) {
        free(env->full_path);
        env->full_path = NULL;
    }
}

void free_string_array(char **strings) {
    for (int i = 0; strings[i] != NULL; i++) {
        free(strings[i]);
    }
}