#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "shell.h"

#define ARGS_SIZE 16

void parse_line(environment_var *env) {
    int bufsize = ARGS_SIZE;
    int length = strlen(env->line);
    env->args = malloc(sizeof(char *) * bufsize);
    if (env->args == NULL) {
        fprintf(stderr, "shell: Memory allocation error\n");
        return;
    }
    int position = 0;
    env->args[position++] = strdup(env->line);

    // Parsing loop
    for (int i = 0; i < length; i++) {
        // If met a whitespace, we can place a NULL terminator
        if (isspace(env->line[i])) {
            env->line[i] = '\0';
            // If the second char isn't whitespace, we add an argument
            if (!isspace(env->line[i + 1])) {
                env->args[position++] = strdup(&env->line[i + 1]);

                // Overflow handling
                if (position >= bufsize) {
                    bufsize += ARGS_SIZE;
                    char **temp = realloc(env->args, sizeof(char *) * bufsize);
                    if (temp == NULL) {
                        fprintf(stderr, "shell: Memory allocation error");
                        free(env->args);
                        env->args = NULL;
                        return;
                    }
                    env->args = temp;
                }
            }
        }
    }

    env->args[position] = NULL;
}

void parse_path(environment_var *env) {
    if (env->args[0] == NULL) {
        return;
    }

    char *path_string = strdup(getenv("PATH"));
    char *dir = strtok(path_string, ":");
    char *path_buffer = malloc(strlen(env->args[0]) + strlen(path_string) + 2);

    while (dir != NULL) {
        sprintf(path_buffer, "%s/%s", dir, env->args[0]);

        if (access(path_buffer, F_OK | X_OK) == 0) {
            env->full_path = path_buffer;
            free(path_string);
            return;
        }

        dir = strtok(NULL, ":");
    }

    free(path_buffer);
    free(path_string);
    env->full_path = NULL;
}
