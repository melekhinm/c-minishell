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
    char *current_token = NULL;

    // Parsing loop
    for (int i = 0; i <= length; i++) {
        char c = env->line[i];

        if (!isspace(c) && c != '\0' && current_token == NULL)
            current_token = &env->line[i];

        // If met a whitespace, we can place a NULL terminator
        else if ((isspace(c) || c == '\0') && current_token != NULL) {
            env->line[i] = '\0';
            env->args[position++] = strdup(current_token);
            current_token = NULL;

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

    env->args[position] = NULL;
}

char *locate_executable(char *path_env, char *file) {
    if (file == NULL) {
        return NULL;
    }

    char *path_string = strdup(path_env);
    char *dir = strtok(path_string, ":");
    char *path_buffer = malloc(strlen(file) + strlen(path_string) + 2);

    while (dir != NULL) {
        sprintf(path_buffer, "%s/%s", dir, file);

        if (access(path_buffer, F_OK | X_OK) == 0) {
            free(path_string);
            return path_buffer;
        }

        dir = strtok(NULL, ":");
    }

    free(path_buffer);
    free(path_string);
    return NULL;
}
