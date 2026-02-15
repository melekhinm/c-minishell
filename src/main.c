#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

void free_env(environment_var *env);
void free_string_array(char **strings);

int main() {
    int status;

    environment_var *env = malloc(sizeof(environment_var));
    if (env == NULL) {
        fprintf(stderr, "shell: Could not start\n");
        exit(EXIT_FAILURE);
    }
    env->ofile = NULL;

    rl_attempted_completion_function = my_completion;
    env->home_dir = strdup(getenv("HOME"));
    shell_read_history(env->home_dir);

    do {
        env->path_env = strdup(getenv("PATH"));
        env->redirection = NOT_REDIRECTING;
        env->line = readline("$ ");
        if (*env->line == '\0') {
            free_env(env);
            continue;
        }

        add_history(env->line);

        char **pipes = check_for_pipeline(env->line);
        if (pipes != NULL) {
            status = execute_pipelines(pipes, env);
        } else {
            parse_line(env);
            status = shell_execute(env);
        }

        free_env(env);
    } while (status);

    shell_write_history(env->home_dir);
    free(env->home_dir);
    free(env);

    exit(EXIT_SUCCESS);
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

    if (env->path_env != NULL) {
        free(env->path_env);
        env->path_env = NULL;
    }

    if (env->ofile != NULL) {
        free(env->ofile);
        env->ofile = NULL;
    }
}

void free_string_array(char **strings) {
    for (int i = 0; strings[i] != NULL; i++) {
        free(strings[i]);
    }
}
