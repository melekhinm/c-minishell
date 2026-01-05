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
        exit(1);
    }
    env->ofile = NULL;

    rl_attempted_completion_function = my_completion;

    // TODO: Consider freeing home_dir and path_env AFTER the loop to
    // avoid adding unnecessary performance overhead.
    do {
        env->home_dir = strdup(getenv("HOME"));
        env->path_env = strdup(getenv("PATH"));
        env->redirection = NOT_REDIRECTING;
        env->line = readline("$ ");
        if (env->line != NULL) {
            add_history(env->line);
        }
        parse_line(env);
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